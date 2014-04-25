#include "test_helper_functions.h"

#define LOCAL_PATH_1    "/home/user/my_directory"
#define VIRTUAL_PATH_1  "my_virtual_dir"
#define NOT_MAPPED_VIRTUAL_DIR  "inexisting_virtual_dir"
#define LOCAL_PATH_2    "/home/other_user/other_my_directory"
#define VIRTUAL_PATH_2  "my_virtual_dir_2"
#define ROOT_PATH   "/"
#define DIR_SEPARATOR "/"
#define FILENAME    "file1.txt"
#define SUBDIR      "subdir"

typedef struct {
    ccapi_fs_virtual_dir_t * dir_entry;
} ccapi_fs_virtual_rootdir_listing_handle_t;

TEST_GROUP(test_ccapi_fs_virtual_drive_map_to_ccfsm)
{
    void setup()
    {
        ccapi_start_t start = {0};
        ccapi_start_error_t start_error;
        ccapi_filesystem_service_t fs_service = {NULL, NULL};
        ccapi_fs_error_t error;
        ccimp_fs_dir_open_t ccimp_dir_open_data_1, ccimp_dir_open_data_2;
        ccimp_fs_dir_close_t ccimp_dir_close_data_1, ccimp_dir_close_data_2;
        void * malloc_for_dir_list_entry_1 = malloc(sizeof (ccapi_fs_virtual_dir_t));
        void * malloc_for_local_path_1 = malloc(sizeof LOCAL_PATH_1);
        void * malloc_for_virtual_path_1 = malloc(sizeof VIRTUAL_PATH_1);
        void * malloc_for_dir_list_entry_2 = malloc(sizeof (ccapi_fs_virtual_dir_t));;
        void * malloc_for_local_path_2 = malloc(sizeof LOCAL_PATH_2);
        void * malloc_for_virtual_path_2 = malloc(sizeof VIRTUAL_PATH_2);

        Mock_create_all();

        th_fill_start_structure_with_good_parameters(&start);
        start.service.file_system = &fs_service;

        start_error = ccapi_start(&start);
        CHECK(start_error == CCAPI_START_ERROR_NONE);
        /* Simulate that imp_context was previously set by other call (file_open) */
        ccapi_data_single_instance->service.file_system.imp_context = &my_fs_context;

        Mock_ccimp_os_malloc_expectAndReturn(sizeof (ccapi_fs_virtual_dir_t), malloc_for_dir_list_entry_1);
        Mock_ccimp_os_malloc_expectAndReturn(sizeof LOCAL_PATH_1, malloc_for_local_path_1);
        Mock_ccimp_os_malloc_expectAndReturn(sizeof VIRTUAL_PATH_1, malloc_for_virtual_path_1);
        Mock_ccimp_os_malloc_expectAndReturn(sizeof (ccapi_fs_virtual_dir_t), malloc_for_dir_list_entry_2);
        Mock_ccimp_os_malloc_expectAndReturn(sizeof LOCAL_PATH_2, malloc_for_local_path_2);
        Mock_ccimp_os_malloc_expectAndReturn(sizeof VIRTUAL_PATH_2, malloc_for_virtual_path_2);

        th_filesystem_prepare_ccimp_dir_open_data_call(&ccimp_dir_open_data_1, LOCAL_PATH_1);
        th_filesystem_prepare_ccimp_dir_close_call(&ccimp_dir_close_data_1);
        th_filesystem_prepare_ccimp_dir_open_data_call(&ccimp_dir_open_data_2, LOCAL_PATH_2);
        th_filesystem_prepare_ccimp_dir_close_call(&ccimp_dir_close_data_2);

        error = ccapi_fs_add_virtual_dir(VIRTUAL_PATH_1, LOCAL_PATH_1);
        CHECK_EQUAL(CCAPI_FS_ERROR_NONE, error);

        error = ccapi_fs_add_virtual_dir(VIRTUAL_PATH_2, LOCAL_PATH_2);
        CHECK_EQUAL(CCAPI_FS_ERROR_NONE, error);
    }

    void teardown()
    {
        Mock_destroy_all();
    }
};

TEST(test_ccapi_fs_virtual_drive_map_to_ccfsm, testRootDirList)
{
    connector_request_id_t request;
    connector_file_system_stat_t ccfsm_filesystem_stat_data;
    connector_file_system_opendir_t ccfsm_dir_open_data;
    connector_file_system_readdir_t ccfsm_dir_read_entry_data;
    connector_file_system_stat_dir_entry_t ccfsm_dir_entry_status_data;
    connector_file_system_close_t ccfsm_dir_close_data;
    connector_callback_status_t status;
    ccapi_fs_virtual_rootdir_listing_handle_t * root_dir_listing_handle = NULL;
    static char buffer[250] = {0};
    static char aux_buffer[250] = {0};

    /* Stat root path */

    ccfsm_filesystem_stat_data.user_context = NULL;
    ccfsm_filesystem_stat_data.path = ROOT_PATH;
    ccfsm_filesystem_stat_data.hash_algorithm.requested = connector_file_system_hash_none;

    request.file_system_request = connector_request_id_file_system_stat;
    status = ccapi_connector_callback(connector_class_id_file_system, request, &ccfsm_filesystem_stat_data, ccapi_data_single_instance);
    CHECK_EQUAL(connector_callback_continue, status);
    CHECK(ccfsm_filesystem_stat_data.user_context != NULL);
    CHECK_EQUAL(0, ccfsm_filesystem_stat_data.statbuf.file_size);
    CHECK_EQUAL(0, ccfsm_filesystem_stat_data.statbuf.last_modified);
    CHECK_EQUAL(connector_file_system_file_type_is_dir, ccfsm_filesystem_stat_data.statbuf.flags);
    CHECK_EQUAL(connector_file_system_hash_none, ccfsm_filesystem_stat_data.hash_algorithm.actual);

    /* Open root path */
    ccfsm_dir_open_data.path = ROOT_PATH;
    ccfsm_dir_open_data.handle = NULL;
    ccfsm_dir_open_data.user_context = ccfsm_filesystem_stat_data.user_context;

    request.file_system_request = connector_request_id_file_system_opendir;
    status = ccapi_connector_callback(connector_class_id_file_system, request, &ccfsm_dir_open_data, ccapi_data_single_instance);
    root_dir_listing_handle = (ccapi_fs_virtual_rootdir_listing_handle_t *)ccfsm_dir_open_data.user_context;

    CHECK_EQUAL(connector_callback_continue, status);
    CHECK_EQUAL(ccapi_data_single_instance->service.file_system.virtual_dir_list, root_dir_listing_handle->dir_entry);
    CHECK(NULL != ccfsm_dir_open_data.handle);

    /* Read first entry and check that it's the second mapped path */
    ccfsm_dir_read_entry_data.handle = ccfsm_dir_open_data.handle;
    ccfsm_dir_read_entry_data.user_context = ccfsm_filesystem_stat_data.user_context;
    ccfsm_dir_read_entry_data.entry_name = buffer;
    ccfsm_dir_read_entry_data.bytes_available = sizeof buffer;

    request.file_system_request = connector_request_id_file_system_readdir;
    status = ccapi_connector_callback(connector_class_id_file_system, request, &ccfsm_dir_read_entry_data, ccapi_data_single_instance);
    root_dir_listing_handle = (ccapi_fs_virtual_rootdir_listing_handle_t *)ccfsm_dir_read_entry_data.user_context;

    CHECK_EQUAL(connector_callback_continue, status);
    STRCMP_EQUAL(VIRTUAL_PATH_2, ccfsm_dir_read_entry_data.entry_name);
    CHECK_EQUAL(ccapi_data_single_instance->service.file_system.virtual_dir_list->next, root_dir_listing_handle->dir_entry);

    /* Stat first entry and check that it's listed as dir */
    strcpy(aux_buffer, ROOT_PATH);
    strcat(aux_buffer, ccfsm_dir_read_entry_data.entry_name);
    ccfsm_dir_entry_status_data.path = aux_buffer;
    ccfsm_dir_entry_status_data.user_context = ccfsm_filesystem_stat_data.user_context;

    request.file_system_request = connector_request_id_file_system_stat_dir_entry;
    status = ccapi_connector_callback(connector_class_id_file_system, request, &ccfsm_dir_entry_status_data, ccapi_data_single_instance);
    root_dir_listing_handle = (ccapi_fs_virtual_rootdir_listing_handle_t *)ccfsm_dir_entry_status_data.user_context;

    CHECK_EQUAL(connector_callback_continue, status);
    CHECK_EQUAL(connector_file_system_file_type_is_dir, ccfsm_dir_entry_status_data.statbuf.flags);
    CHECK_EQUAL(0, ccfsm_dir_entry_status_data.statbuf.file_size);
    CHECK_EQUAL(0, ccfsm_dir_entry_status_data.statbuf.last_modified);

    /* Read second entry and check that it's the first mapped path */
    request.file_system_request = connector_request_id_file_system_readdir;
    status = ccapi_connector_callback(connector_class_id_file_system, request, &ccfsm_dir_read_entry_data, ccapi_data_single_instance);
    root_dir_listing_handle = (ccapi_fs_virtual_rootdir_listing_handle_t *)ccfsm_dir_read_entry_data.user_context;

    CHECK_EQUAL(connector_callback_continue, status);
    STRCMP_EQUAL(VIRTUAL_PATH_1, ccfsm_dir_read_entry_data.entry_name);
    CHECK(NULL == root_dir_listing_handle->dir_entry);

    /* Stat second entry and check that it's listed as dir */
    strcpy(aux_buffer, ROOT_PATH);
    strcat(aux_buffer, ccfsm_dir_read_entry_data.entry_name);
    ccfsm_dir_entry_status_data.path = aux_buffer;
    ccfsm_dir_entry_status_data.user_context = ccfsm_filesystem_stat_data.user_context;

    request.file_system_request = connector_request_id_file_system_stat_dir_entry;
    status = ccapi_connector_callback(connector_class_id_file_system, request, &ccfsm_dir_entry_status_data, ccapi_data_single_instance);
    root_dir_listing_handle = (ccapi_fs_virtual_rootdir_listing_handle_t *)ccfsm_dir_entry_status_data.user_context;

    CHECK_EQUAL(connector_callback_continue, status);
    CHECK_EQUAL(connector_file_system_file_type_is_dir, ccfsm_dir_entry_status_data.statbuf.flags);
    CHECK_EQUAL(0, ccfsm_dir_entry_status_data.statbuf.file_size);
    CHECK_EQUAL(0, ccfsm_dir_entry_status_data.statbuf.last_modified);

    /* Read last entry and check that it's an empty string */
    request.file_system_request = connector_request_id_file_system_readdir;
    status = ccapi_connector_callback(connector_class_id_file_system, request, &ccfsm_dir_read_entry_data, ccapi_data_single_instance);
    root_dir_listing_handle = (ccapi_fs_virtual_rootdir_listing_handle_t *)ccfsm_dir_read_entry_data.user_context;

    CHECK_EQUAL(connector_callback_continue, status);
    STRCMP_EQUAL("", ccfsm_dir_read_entry_data.entry_name);
    CHECK(NULL == root_dir_listing_handle->dir_entry);

    /* Close directory */
    ccfsm_dir_close_data.user_context = ccfsm_filesystem_stat_data.user_context;
    request.file_system_request = connector_request_id_file_system_closedir;
    status = ccapi_connector_callback(connector_class_id_file_system, request, &ccfsm_dir_close_data, ccapi_data_single_instance);
    root_dir_listing_handle = (ccapi_fs_virtual_rootdir_listing_handle_t *)ccfsm_dir_close_data.user_context;

    CHECK_EQUAL(connector_callback_continue, status);
    CHECK(NULL == ccfsm_dir_close_data.user_context);
}

TEST(test_ccapi_fs_virtual_drive_map_to_ccfsm, testOpenVirtualDirInvalid)
{
    connector_request_id_t request;
    connector_file_system_opendir_t ccfsm_dir_open_data;
    connector_callback_status_t status;
    char const * const ccfsm_path = ROOT_PATH NOT_MAPPED_VIRTUAL_DIR;

    ccfsm_dir_open_data.user_context = NULL;
    ccfsm_dir_open_data.errnum = NULL;
    ccfsm_dir_open_data.handle = NULL;
    ccfsm_dir_open_data.path = ccfsm_path;

    request.file_system_request = connector_request_id_file_system_opendir;
    status = ccapi_connector_callback(connector_class_id_file_system, request, &ccfsm_dir_open_data, ccapi_data_single_instance);

    CHECK_EQUAL(connector_callback_error, status);
    CHECK(NULL == ccfsm_dir_open_data.handle);
}

TEST(test_ccapi_fs_virtual_drive_map_to_ccfsm, testOpenVirtualDir)
{
    connector_request_id_t request;
    ccimp_fs_dir_open_t ccimp_dir_open_data;
    connector_file_system_opendir_t ccfsm_dir_open_data;
    connector_callback_status_t status;
    char const * const ccfsm_path = ROOT_PATH VIRTUAL_PATH_1;
    void * malloc_for_local_path = malloc(sizeof LOCAL_PATH_1);

    th_filesystem_prepare_ccimp_dir_open_data_call(&ccimp_dir_open_data, LOCAL_PATH_1);

    ccfsm_dir_open_data.user_context = NULL;
    ccfsm_dir_open_data.errnum = ccimp_dir_open_data.errnum.pointer;
    ccfsm_dir_open_data.handle = ccimp_dir_open_data.handle.pointer;
    ccfsm_dir_open_data.path = ccfsm_path;

    Mock_ccimp_os_malloc_expectAndReturn(sizeof LOCAL_PATH_1, malloc_for_local_path);
    Mock_ccimp_os_free_expectAndReturn(malloc_for_local_path, CCIMP_STATUS_OK);

    request.file_system_request = connector_request_id_file_system_opendir;
    status = ccapi_connector_callback(connector_class_id_file_system, request, &ccfsm_dir_open_data, ccapi_data_single_instance);

    CHECK_EQUAL(connector_callback_continue, status);
    CHECK(NULL != ccfsm_dir_open_data.handle);
}

TEST(test_ccapi_fs_virtual_drive_map_to_ccfsm, testOpenVirtualDirSubdir)
{
    connector_request_id_t request;
    ccimp_fs_dir_open_t ccimp_dir_open_data;
    connector_file_system_opendir_t ccfsm_dir_open_data;
    connector_callback_status_t status;
    char const * const ccfsm_path = ROOT_PATH VIRTUAL_PATH_1 DIR_SEPARATOR SUBDIR;
    void * malloc_for_local_path = malloc(sizeof LOCAL_PATH_1 DIR_SEPARATOR SUBDIR);

    th_filesystem_prepare_ccimp_dir_open_data_call(&ccimp_dir_open_data, LOCAL_PATH_1 DIR_SEPARATOR SUBDIR);

    ccfsm_dir_open_data.errnum = ccimp_dir_open_data.errnum.pointer;
    ccfsm_dir_open_data.user_context = NULL;
    ccfsm_dir_open_data.handle = ccimp_dir_open_data.handle.pointer;
    ccfsm_dir_open_data.path = ccfsm_path;

    Mock_ccimp_os_malloc_expectAndReturn(sizeof LOCAL_PATH_1 DIR_SEPARATOR SUBDIR, malloc_for_local_path);
    Mock_ccimp_os_free_expectAndReturn(malloc_for_local_path, CCIMP_STATUS_OK);

    request.file_system_request = connector_request_id_file_system_opendir;
    status = ccapi_connector_callback(connector_class_id_file_system, request, &ccfsm_dir_open_data, ccapi_data_single_instance);

    CHECK_EQUAL(connector_callback_continue, status);
    CHECK_EQUAL(6, *(my_filesystem_context_t *)ccapi_data_single_instance->service.file_system.imp_context);
    CHECK(NULL != ccfsm_dir_open_data.handle);
}

TEST(test_ccapi_fs_virtual_drive_map_to_ccfsm, testStatVirtualDirEntryInvalid)
{
    connector_request_id_t request;
    connector_file_system_stat_dir_entry_t ccfsm_dir_entry_status_data;
    connector_callback_status_t status;
    char const * const ccfsm_path = ROOT_PATH NOT_MAPPED_VIRTUAL_DIR;

    ccfsm_dir_entry_status_data.errnum = NULL;
    ccfsm_dir_entry_status_data.user_context = NULL;
    ccfsm_dir_entry_status_data.path = ccfsm_path;
    ccfsm_dir_entry_status_data.statbuf.file_size = 0;
    ccfsm_dir_entry_status_data.statbuf.last_modified = 0;
    ccfsm_dir_entry_status_data.statbuf.flags = connector_file_system_file_type_none;

    request.file_system_request = connector_request_id_file_system_stat_dir_entry;
    status = ccapi_connector_callback(connector_class_id_file_system, request, &ccfsm_dir_entry_status_data, ccapi_data_single_instance);

    CHECK_EQUAL(connector_callback_error, status);
}

TEST(test_ccapi_fs_virtual_drive_map_to_ccfsm, testStatVirtualDirEntry)
{
    connector_request_id_t request;
    ccimp_fs_dir_entry_status_t ccimp_dir_entry_status_data;
    connector_file_system_stat_dir_entry_t ccfsm_dir_entry_status_data;
    connector_callback_status_t status;
    char const * const ccfsm_path = ROOT_PATH VIRTUAL_PATH_1;
    void * malloc_for_local_path = malloc(sizeof LOCAL_PATH_1);

    ccimp_dir_entry_status_data.errnum.pointer = NULL;
    ccimp_dir_entry_status_data.imp_context = &my_fs_context;
    ccimp_dir_entry_status_data.path = LOCAL_PATH_1;
    ccimp_dir_entry_status_data.status.file_size = 0;
    ccimp_dir_entry_status_data.status.last_modified = 0;
    ccimp_dir_entry_status_data.status.type = CCIMP_FS_DIR_ENTRY_UNKNOWN;

    ccfsm_dir_entry_status_data.errnum = ccimp_dir_entry_status_data.errnum.pointer;
    ccfsm_dir_entry_status_data.user_context = NULL;
    ccfsm_dir_entry_status_data.path = ccfsm_path;
    ccfsm_dir_entry_status_data.statbuf.file_size = ccimp_dir_entry_status_data.status.file_size;
    ccfsm_dir_entry_status_data.statbuf.last_modified = ccimp_dir_entry_status_data.status.last_modified;
    ccfsm_dir_entry_status_data.statbuf.flags = connector_file_system_file_type_none;

    Mock_ccimp_fs_dir_entry_status_expectAndReturn(&ccimp_dir_entry_status_data, CCIMP_STATUS_OK);
    Mock_ccimp_os_malloc_expectAndReturn(sizeof LOCAL_PATH_1, malloc_for_local_path);
    Mock_ccimp_os_free_expectAndReturn(malloc_for_local_path, CCIMP_STATUS_OK);

    request.file_system_request = connector_request_id_file_system_stat_dir_entry;
    status = ccapi_connector_callback(connector_class_id_file_system, request, &ccfsm_dir_entry_status_data, ccapi_data_single_instance);

    CHECK_EQUAL(connector_callback_continue, status);
}

TEST(test_ccapi_fs_virtual_drive_map_to_ccfsm, testStatVirtualDirInvalid)
{
    connector_request_id_t request;
    connector_file_system_stat_t ccfsm_hash_status_data;
    connector_callback_status_t status;
    char const * const ccfsm_path = ROOT_PATH NOT_MAPPED_VIRTUAL_DIR "/file1.txt";

    ccfsm_hash_status_data.errnum = NULL;
    ccfsm_hash_status_data.user_context = NULL;
    ccfsm_hash_status_data.path = ccfsm_path;
    ccfsm_hash_status_data.statbuf.file_size = 0;
    ccfsm_hash_status_data.statbuf.last_modified = 0;
    ccfsm_hash_status_data.statbuf.flags = connector_file_system_file_type_none;
    ccfsm_hash_status_data.hash_algorithm.requested = connector_file_system_hash_best;
    ccfsm_hash_status_data.hash_algorithm.actual = connector_file_system_hash_none;

    request.file_system_request = connector_request_id_file_system_stat;
    status = ccapi_connector_callback(connector_class_id_file_system, request, &ccfsm_hash_status_data, ccapi_data_single_instance);

    CHECK_EQUAL(connector_callback_error, status);
}

TEST(test_ccapi_fs_virtual_drive_map_to_ccfsm, testStatVirtualDir)
{
    connector_request_id_t request;
    ccimp_fs_hash_status_t ccimp_hash_status_data;
    connector_file_system_stat_t ccfsm_hash_status_data;
    connector_callback_status_t status;
    char const * const ccfsm_path = ROOT_PATH VIRTUAL_PATH_1 "/file1.txt";
    void * malloc_for_local_path = malloc(sizeof LOCAL_PATH_1 DIR_SEPARATOR FILENAME);

    ccimp_hash_status_data.errnum.pointer = NULL;
    ccimp_hash_status_data.imp_context = &my_fs_context;
    ccimp_hash_status_data.path = LOCAL_PATH_1 DIR_SEPARATOR FILENAME;
    ccimp_hash_status_data.status.file_size = 0;
    ccimp_hash_status_data.status.last_modified = 0;
    ccimp_hash_status_data.status.type = CCIMP_FS_DIR_ENTRY_UNKNOWN;
    ccimp_hash_status_data.hash_alg.requested = CCIMP_FS_HASH_BEST;
    ccimp_hash_status_data.hash_alg.actual = CCIMP_FS_HASH_NONE;

    ccfsm_hash_status_data.errnum = ccimp_hash_status_data.errnum.pointer;
    ccfsm_hash_status_data.user_context = NULL;
    ccfsm_hash_status_data.path = ccfsm_path;
    ccfsm_hash_status_data.statbuf.file_size = ccimp_hash_status_data.status.file_size;
    ccfsm_hash_status_data.statbuf.last_modified = ccimp_hash_status_data.status.last_modified;
    ccfsm_hash_status_data.statbuf.flags = connector_file_system_file_type_none;
    ccfsm_hash_status_data.hash_algorithm.requested = connector_file_system_hash_best;
    ccfsm_hash_status_data.hash_algorithm.actual = connector_file_system_hash_none;

    Mock_ccimp_fs_hash_status_expectAndReturn(&ccimp_hash_status_data, CCIMP_STATUS_OK);
    Mock_ccimp_os_malloc_expectAndReturn(sizeof LOCAL_PATH_1 DIR_SEPARATOR FILENAME, malloc_for_local_path);
    Mock_ccimp_os_free_expectAndReturn(malloc_for_local_path, CCIMP_STATUS_OK);

    request.file_system_request = connector_request_id_file_system_stat;
    status = ccapi_connector_callback(connector_class_id_file_system, request, &ccfsm_hash_status_data, ccapi_data_single_instance);

    CHECK_EQUAL(connector_callback_continue, status);
}

TEST(test_ccapi_fs_virtual_drive_map_to_ccfsm, testRemoveFileFromVirtualDirInvalid)
{
    connector_request_id_t request;
    connector_file_system_remove_t ccfsm_file_remove_data;
    connector_callback_status_t status;
    char const * const ccfsm_path = ROOT_PATH NOT_MAPPED_VIRTUAL_DIR DIR_SEPARATOR FILENAME;

    ccfsm_file_remove_data.errnum = NULL;
    ccfsm_file_remove_data.user_context = NULL;
    ccfsm_file_remove_data.path = ccfsm_path;

    request.file_system_request = connector_request_id_file_system_remove;
    status = ccapi_connector_callback(connector_class_id_file_system, request, &ccfsm_file_remove_data, ccapi_data_single_instance);

    CHECK_EQUAL(connector_callback_error, status);
}

TEST(test_ccapi_fs_virtual_drive_map_to_ccfsm, testRemoveFileFromVirtualDir)
{
    connector_request_id_t request;
    ccimp_fs_file_remove_t ccimp_file_remove_data;
    connector_file_system_remove_t ccfsm_file_remove_data;
    connector_callback_status_t status;
    char const * const ccfsm_path = ROOT_PATH VIRTUAL_PATH_1 DIR_SEPARATOR FILENAME;
    void * malloc_for_local_path = malloc(sizeof LOCAL_PATH_1 DIR_SEPARATOR FILENAME);

    ccimp_file_remove_data.errnum.pointer = NULL;
    ccimp_file_remove_data.imp_context = &my_fs_context;
    ccimp_file_remove_data.path = LOCAL_PATH_1 DIR_SEPARATOR FILENAME;

    ccfsm_file_remove_data.errnum = ccimp_file_remove_data.errnum.pointer;
    ccfsm_file_remove_data.user_context = NULL;
    ccfsm_file_remove_data.path = ccfsm_path;

    Mock_ccimp_fs_file_remove_expectAndReturn(&ccimp_file_remove_data, CCIMP_STATUS_OK);
    Mock_ccimp_os_malloc_expectAndReturn(sizeof LOCAL_PATH_1 DIR_SEPARATOR FILENAME, malloc_for_local_path);
    Mock_ccimp_os_free_expectAndReturn(malloc_for_local_path, CCIMP_STATUS_OK);

    request.file_system_request = connector_request_id_file_system_remove;
    status = ccapi_connector_callback(connector_class_id_file_system, request, &ccfsm_file_remove_data, ccapi_data_single_instance);

    CHECK_EQUAL(connector_callback_continue, status);
}

TEST(test_ccapi_fs_virtual_drive_map_to_ccfsm, testOpenFileFromVirtualDirInvalid)
{
    connector_request_id_t request;
    connector_file_system_open_t ccfsm_file_open_data;
    connector_callback_status_t status;
    char const * const ccfsm_path = ROOT_PATH NOT_MAPPED_VIRTUAL_DIR DIR_SEPARATOR FILENAME;

    ccfsm_file_open_data.path = ccfsm_path;
    ccfsm_file_open_data.errnum = NULL;
    ccfsm_file_open_data.user_context = NULL;
    ccfsm_file_open_data.handle = NULL;
    ccfsm_file_open_data.oflag = CONNECTOR_FILE_O_RDWR | CONNECTOR_FILE_O_APPEND;

    request.file_system_request = connector_request_id_file_system_open;
    status = ccapi_connector_callback(connector_class_id_file_system, request, &ccfsm_file_open_data, ccapi_data_single_instance);

    CHECK_EQUAL(connector_callback_error, status);
}

TEST(test_ccapi_fs_virtual_drive_map_to_ccfsm, testOpenFileFromVirtualDir)
{
    connector_request_id_t request;
    ccimp_fs_file_open_t ccimp_file_open_data;
    connector_file_system_open_t ccfsm_file_open_data;
    connector_callback_status_t status;
    char const * const ccfsm_path = ROOT_PATH VIRTUAL_PATH_1 DIR_SEPARATOR FILENAME;

    void * malloc_for_local_path = malloc(sizeof LOCAL_PATH_1 DIR_SEPARATOR FILENAME);
    void * malloc_for_ccapi_fs_handle = malloc(sizeof (ccapi_fs_file_handle_t));
    void * malloc_for_path = malloc(sizeof LOCAL_PATH_1 DIR_SEPARATOR FILENAME);

    ccimp_file_open_data.path = LOCAL_PATH_1 DIR_SEPARATOR FILENAME;
    ccimp_file_open_data.errnum.pointer = NULL;
    ccimp_file_open_data.imp_context = &my_fs_context;
    ccimp_file_open_data.handle.pointer = NULL;
    ccimp_file_open_data.flags = CCIMP_FILE_O_RDWR | CCIMP_FILE_O_APPEND;

    ccfsm_file_open_data.path = ccfsm_path;
    ccfsm_file_open_data.errnum = ccimp_file_open_data.errnum.pointer;
    ccfsm_file_open_data.user_context = NULL;
    ccfsm_file_open_data.handle = ccimp_file_open_data.handle.pointer;
    ccfsm_file_open_data.oflag = CONNECTOR_FILE_O_RDWR | CONNECTOR_FILE_O_APPEND;

    Mock_ccimp_fs_file_open_expectAndReturn(&ccimp_file_open_data, CCIMP_STATUS_OK);
    Mock_ccimp_os_malloc_expectAndReturn(sizeof LOCAL_PATH_1 DIR_SEPARATOR FILENAME, malloc_for_local_path);
    Mock_ccimp_os_malloc_expectAndReturn(sizeof (ccapi_fs_file_handle_t), malloc_for_ccapi_fs_handle);
    Mock_ccimp_os_malloc_expectAndReturn(sizeof LOCAL_PATH_1 DIR_SEPARATOR FILENAME, malloc_for_path);
    Mock_ccimp_os_free_expectAndReturn(malloc_for_local_path, CCIMP_STATUS_OK);

    request.file_system_request = connector_request_id_file_system_open;
    status = ccapi_connector_callback(connector_class_id_file_system, request, &ccfsm_file_open_data, ccapi_data_single_instance);

    CHECK_EQUAL(connector_callback_continue, status);
}

TEST(test_ccapi_fs_virtual_drive_map_to_ccfsm, testHashFileFromVirtualDirInvalid)
{
    connector_request_id_t request;
    ccimp_fs_hash_file_t ccimp_hash_file_data;
    connector_file_system_hash_t ccfsm_hash_file_data;
    connector_callback_status_t status;
    uint32_t hash_value = 0;
    char const * const ccfsm_path = ROOT_PATH NOT_MAPPED_VIRTUAL_DIR DIR_SEPARATOR FILENAME;

    ccimp_hash_file_data.path = LOCAL_PATH_1 DIR_SEPARATOR FILENAME;
    ccimp_hash_file_data.errnum.pointer = NULL;
    ccimp_hash_file_data.imp_context = &my_fs_context;
    ccimp_hash_file_data.hash_algorithm = CCIMP_FS_HASH_CRC32;
    ccimp_hash_file_data.hash_value = &hash_value;
    ccimp_hash_file_data.bytes_requested = sizeof hash_value;

    ccfsm_hash_file_data.path = ccfsm_path;
    ccfsm_hash_file_data.errnum = ccimp_hash_file_data.errnum.pointer;
    ccfsm_hash_file_data.user_context = NULL;
    ccfsm_hash_file_data.hash_algorithm = connector_file_system_hash_crc32;
    ccfsm_hash_file_data.hash_value = ccimp_hash_file_data.hash_value;
    ccfsm_hash_file_data.bytes_requested = ccimp_hash_file_data.bytes_requested;

    request.file_system_request = connector_request_id_file_system_hash;
    status = ccapi_connector_callback(connector_class_id_file_system, request, &ccfsm_hash_file_data, ccapi_data_single_instance);

    CHECK_EQUAL(connector_callback_error, status);
}

TEST(test_ccapi_fs_virtual_drive_map_to_ccfsm, testHashFileFromVirtualDir)
{
    connector_request_id_t request;
    ccimp_fs_hash_file_t ccimp_hash_file_data;
    connector_file_system_hash_t ccfsm_hash_file_data;
    connector_callback_status_t status;
    uint32_t hash_value = 0;
    char const * const ccfsm_path = ROOT_PATH VIRTUAL_PATH_1 DIR_SEPARATOR FILENAME;
    void * malloc_for_local_path = malloc(sizeof LOCAL_PATH_1 DIR_SEPARATOR FILENAME);

    ccimp_hash_file_data.path = LOCAL_PATH_1 DIR_SEPARATOR FILENAME;
    ccimp_hash_file_data.errnum.pointer = NULL;
    ccimp_hash_file_data.imp_context = &my_fs_context;
    ccimp_hash_file_data.hash_algorithm = CCIMP_FS_HASH_CRC32;
    ccimp_hash_file_data.hash_value = &hash_value;
    ccimp_hash_file_data.bytes_requested = sizeof hash_value;

    ccfsm_hash_file_data.path = ccfsm_path;
    ccfsm_hash_file_data.errnum = ccimp_hash_file_data.errnum.pointer;
    ccfsm_hash_file_data.user_context = NULL;
    ccfsm_hash_file_data.hash_algorithm = connector_file_system_hash_crc32;
    ccfsm_hash_file_data.hash_value = ccimp_hash_file_data.hash_value;
    ccfsm_hash_file_data.bytes_requested = ccimp_hash_file_data.bytes_requested;

    Mock_ccimp_fs_hash_file_expectAndReturn(&ccimp_hash_file_data, CCIMP_STATUS_OK);
    Mock_ccimp_os_malloc_expectAndReturn(sizeof LOCAL_PATH_1 DIR_SEPARATOR FILENAME, malloc_for_local_path);
    Mock_ccimp_os_free_expectAndReturn(malloc_for_local_path, CCIMP_STATUS_OK);

    request.file_system_request = connector_request_id_file_system_hash;
    status = ccapi_connector_callback(connector_class_id_file_system, request, &ccfsm_hash_file_data, ccapi_data_single_instance);

    CHECK_EQUAL(connector_callback_continue, status);
}