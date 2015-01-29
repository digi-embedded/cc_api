/*
 * This is an auto-generated file - DO NOT EDIT! 
 * This is a C file generated by RCI Generator tool.
 * This file was generated on: 2015/01/29 19:18:32 
 * The command line arguments were: "username:***** "Linux Application" 1.0.0.0-vendor=0x2001371 -noUpload -noBackup -usenames=all -ccapiStub  config.rci"
 * The version of RCI Generator tool was: 2.0.0.0 */

#include  <stdio.h>
#include  "ccapi/ccapi.h"
#include  "ccapi_rci_functions.h"

#if !(defined ASSERT)
#define ASSERT(cond)       assert((cond))
#endif

ccapi_global_error_id_t rci_session_start_cb(ccapi_rci_info_t * const info)
{
    ASSERT(info->action == CCAPI_RCI_ACTION_QUERY);
    ASSERT(info->group.instance == 0);
    ASSERT(info->group.name == NULL);
    ASSERT(info->group.type == CCAPI_RCI_GROUP_SETTING);
    ASSERT(info->element.name == NULL);
    ASSERT(info->query_setting.attributes.compare_to == CCAPI_RCI_QUERY_SETTING_ATTRIBUTE_COMPARE_TO_NONE);
    ASSERT(info->query_setting.attributes.source == CCAPI_RCI_QUERY_SETTING_ATTRIBUTE_SOURCE_CURRENT);
    ASSERT(info->query_setting.matches == CCAPI_FALSE);
    ASSERT(info->error_hint == NULL);
    ASSERT(info->user_context == NULL);
    return CCAPI_GLOBAL_ERROR_NONE;
}

ccapi_global_error_id_t rci_session_end_cb(ccapi_rci_info_t * const info)
{
    ASSERT(info->error_hint == NULL);
    ASSERT(info->user_context == NULL);
    return CCAPI_GLOBAL_ERROR_NONE;
}

ccapi_global_error_id_t rci_action_start_cb(ccapi_rci_info_t * const info)
{
    printf("%s: info->action %d\n", __FUNCTION__, info->action);
    printf("%s: info->type %d\n", __FUNCTION__, info->group.type);

    if (info->action == CCAPI_RCI_ACTION_QUERY && info->group.type == CCAPI_RCI_GROUP_SETTING)
    {
        printf("%s: info->query_setting.attributes.compare_to %d\n", __FUNCTION__, info->query_setting.attributes.compare_to);
        printf("%s: info->query_setting.attributes.source %d\n", __FUNCTION__, info->query_setting.attributes.source);
    }
    else
    {
        ASSERT(info->query_setting.attributes.compare_to == CCAPI_RCI_QUERY_SETTING_ATTRIBUTE_COMPARE_TO_NONE);
        ASSERT(info->query_setting.attributes.source == CCAPI_RCI_QUERY_SETTING_ATTRIBUTE_SOURCE_CURRENT);
    }
    ASSERT(info->group.instance == 0);
    ASSERT(info->group.name == NULL);
    ASSERT(info->element.name == NULL);
    ASSERT(info->query_setting.matches == CCAPI_FALSE);
    ASSERT(info->error_hint == NULL);
    ASSERT(info->user_context == NULL);
    return CCAPI_GLOBAL_ERROR_NONE;
}

ccapi_global_error_id_t rci_action_end_cb(ccapi_rci_info_t * const info)
{
    printf("%s: info->action %d\n", __FUNCTION__, info->action);
    printf("%s: info->type %d\n", __FUNCTION__, info->group.type);

    if (info->action == CCAPI_RCI_ACTION_QUERY && info->group.type == CCAPI_RCI_GROUP_SETTING)
    {
        printf("%s: info->query_setting.attributes.compare_to %d\n", __FUNCTION__, info->query_setting.attributes.compare_to);
        printf("%s: info->query_setting.attributes.source %d\n", __FUNCTION__, info->query_setting.attributes.source);
    }
    else
    {
        ASSERT(info->query_setting.attributes.compare_to == CCAPI_RCI_QUERY_SETTING_ATTRIBUTE_COMPARE_TO_NONE);
        ASSERT(info->query_setting.attributes.source == CCAPI_RCI_QUERY_SETTING_ATTRIBUTE_SOURCE_CURRENT);
    }
    ASSERT(info->error_hint == NULL);
    ASSERT(info->user_context == NULL);
    return CCAPI_GLOBAL_ERROR_NONE;
}

ccapi_global_error_id_t rci_do_command_cb(ccapi_rci_info_t * const info)
{
    ASSERT(info->action == CCAPI_RCI_ACTION_DO_COMMAND);
    ASSERT(info->group.instance == 0);
    ASSERT(info->group.name == NULL);
    ASSERT(info->group.type == CCAPI_RCI_GROUP_SETTING);
    ASSERT(info->element.name == NULL);
    ASSERT(info->query_setting.attributes.compare_to == CCAPI_RCI_QUERY_SETTING_ATTRIBUTE_COMPARE_TO_NONE);
    ASSERT(info->query_setting.attributes.source == CCAPI_RCI_QUERY_SETTING_ATTRIBUTE_SOURCE_CURRENT);
    ASSERT(info->query_setting.matches == CCAPI_FALSE);
    ASSERT(info->error_hint == NULL);
    ASSERT(info->user_context == NULL);
    return CCAPI_GLOBAL_ERROR_NONE;
}

ccapi_global_error_id_t rci_set_factory_defaults_cb(ccapi_rci_info_t * const info)
{
    ASSERT(info->action == CCAPI_RCI_ACTION_SET_FACTORY_DEFAULTS);
    ASSERT(info->group.instance == 0);
    ASSERT(info->group.name == NULL);
    ASSERT(info->group.type == CCAPI_RCI_GROUP_SETTING);
    ASSERT(info->element.name == NULL);
    ASSERT(info->query_setting.attributes.compare_to == CCAPI_RCI_QUERY_SETTING_ATTRIBUTE_COMPARE_TO_NONE);
    ASSERT(info->query_setting.attributes.source == CCAPI_RCI_QUERY_SETTING_ATTRIBUTE_SOURCE_CURRENT);
    ASSERT(info->query_setting.matches == CCAPI_FALSE);
    ASSERT(info->error_hint == NULL);
    ASSERT(info->user_context == NULL);
    return CCAPI_GLOBAL_ERROR_NONE;
}

ccapi_global_error_id_t rci_reboot_cb(ccapi_rci_info_t * const info)
{
    ASSERT(info->action == CCAPI_RCI_ACTION_REBOOT);
    ASSERT(info->group.instance == 0);
    ASSERT(info->group.name == NULL);
    ASSERT(info->group.type == CCAPI_RCI_GROUP_SETTING);
    ASSERT(info->element.name == NULL);
    ASSERT(info->query_setting.attributes.compare_to == CCAPI_RCI_QUERY_SETTING_ATTRIBUTE_COMPARE_TO_NONE);
    ASSERT(info->query_setting.attributes.source == CCAPI_RCI_QUERY_SETTING_ATTRIBUTE_SOURCE_CURRENT);
    ASSERT(info->query_setting.matches == CCAPI_FALSE);
    ASSERT(info->error_hint == NULL);
    ASSERT(info->user_context == NULL);
    return CCAPI_GLOBAL_ERROR_NONE;
}

ccapi_setting_group_1_error_id_t rci_setting_group_1_start(ccapi_rci_info_t * const info)
{
    printf("%s: info->action %d\n", __FUNCTION__, info->action);

    if (info->action == CCAPI_RCI_ACTION_QUERY && info->group.type == CCAPI_RCI_GROUP_SETTING)
    {
        printf("%s: info->query_setting.attributes.compare_to %d\n", __FUNCTION__, info->query_setting.attributes.compare_to);
        printf("%s: info->query_setting.attributes.source %d\n", __FUNCTION__, info->query_setting.attributes.source);
    }
    else
    {
        ASSERT(info->query_setting.attributes.compare_to == CCAPI_RCI_QUERY_SETTING_ATTRIBUTE_COMPARE_TO_NONE);
        ASSERT(info->query_setting.attributes.source == CCAPI_RCI_QUERY_SETTING_ATTRIBUTE_SOURCE_CURRENT);
    }
    printf("%s: info->group.instance %d\n", __FUNCTION__, info->group.instance);
    ASSERT(info->group.type == CCAPI_RCI_GROUP_SETTING);
    ASSERT(strcmp(info->group.name, "group_1") == 0);
    ASSERT(info->element.name == NULL);
    ASSERT(info->query_setting.matches == CCAPI_FALSE);
    ASSERT(info->error_hint == NULL);
    ASSERT(info->user_context == NULL);

    return CCAPI_GLOBAL_ERROR_NONE;
}

ccapi_setting_group_1_error_id_t rci_setting_group_1_end(ccapi_rci_info_t * const info)
{
    printf("%s: info->action %d\n", __FUNCTION__, info->action);

    if (info->action == CCAPI_RCI_ACTION_QUERY && info->group.type == CCAPI_RCI_GROUP_SETTING)
    {
        printf("%s: info->query_setting.attributes.compare_to %d\n", __FUNCTION__, info->query_setting.attributes.compare_to);
        printf("%s: info->query_setting.attributes.source %d\n", __FUNCTION__, info->query_setting.attributes.source);
    }
    else
    {
        ASSERT(info->query_setting.attributes.compare_to == CCAPI_RCI_QUERY_SETTING_ATTRIBUTE_COMPARE_TO_NONE);
        ASSERT(info->query_setting.attributes.source == CCAPI_RCI_QUERY_SETTING_ATTRIBUTE_SOURCE_CURRENT);
    }
    printf("%s: info->group.instance %d\n", __FUNCTION__, info->group.instance);
    ASSERT(info->group.type == CCAPI_RCI_GROUP_SETTING);
    ASSERT(strcmp(info->group.name, "group_1") == 0);
    ASSERT(info->query_setting.matches == CCAPI_FALSE);
    ASSERT(info->error_hint == NULL);
    ASSERT(info->user_context == NULL);

    return CCAPI_GLOBAL_ERROR_NONE;
}

ccapi_setting_group_1_error_id_t rci_setting_group_1_el_enum_get(ccapi_rci_info_t * const info, ccapi_setting_group_1_el_enum_id_t * const value)
{
    ASSERT(info->action == CCAPI_RCI_ACTION_QUERY);

    printf("%s: info->query_setting.attributes.compare_to %d\n", __FUNCTION__, info->query_setting.attributes.compare_to);
    printf("%s: info->query_setting.attributes.source %d\n", __FUNCTION__, info->query_setting.attributes.source);
    printf("%s: info->group.instance %d\n", __FUNCTION__, info->group.instance);
    ASSERT(info->group.type == CCAPI_RCI_GROUP_SETTING);
    ASSERT(strcmp(info->group.name, "group_1") == 0);
    ASSERT(strcmp(info->element.name, "el_enum") == 0);
    ASSERT(info->query_setting.matches == CCAPI_FALSE);
    ASSERT(info->error_hint == NULL);
    ASSERT(info->user_context == NULL);

    *value = CCAPI_SETTING_GROUP_1_EL_ENUM_ONE;
    return CCAPI_GLOBAL_ERROR_NONE;
}

ccapi_setting_group_1_error_id_t rci_setting_group_1_el_enum_set(ccapi_rci_info_t * const info, ccapi_setting_group_1_el_enum_id_t const *const value)
{
    ASSERT(info->action == CCAPI_RCI_ACTION_SET);

    ASSERT(info->query_setting.attributes.compare_to == CCAPI_RCI_QUERY_SETTING_ATTRIBUTE_COMPARE_TO_NONE);
    ASSERT(info->query_setting.attributes.source == CCAPI_RCI_QUERY_SETTING_ATTRIBUTE_SOURCE_CURRENT);
    ASSERT(info->group.type == CCAPI_RCI_GROUP_SETTING);
    ASSERT(strcmp(info->group.name, "group_1") == 0);
    ASSERT(strcmp(info->element.name, "el_enum") == 0);
    ASSERT(info->query_setting.matches == CCAPI_FALSE);
    ASSERT(info->error_hint == NULL);
    ASSERT(info->user_context == NULL);

    UNUSED_PARAMETER(value);
    return CCAPI_GLOBAL_ERROR_NONE;
}

ccapi_setting_group_1_error_id_t rci_setting_group_1_el_uint32_get(ccapi_rci_info_t * const info, uint32_t * const value)
{
    ASSERT(info->action == CCAPI_RCI_ACTION_QUERY);

    printf("%s: info->query_setting.attributes.compare_to %d\n", __FUNCTION__, info->query_setting.attributes.compare_to);
    printf("%s: info->query_setting.attributes.source %d\n", __FUNCTION__, info->query_setting.attributes.source);
    printf("%s: info->group.instance %d\n", __FUNCTION__, info->group.instance);
    ASSERT(info->group.type == CCAPI_RCI_GROUP_SETTING);
    ASSERT(strcmp(info->group.name, "group_1") == 0);
    ASSERT(strcmp(info->element.name, "el_uint32") == 0);
    ASSERT(info->query_setting.matches == CCAPI_FALSE);
    ASSERT(info->error_hint == NULL);
    ASSERT(info->user_context == NULL);
    *value = 5;
    return CCAPI_GLOBAL_ERROR_NONE;
}

ccapi_setting_group_1_error_id_t rci_setting_group_1_el_uint32_set(ccapi_rci_info_t * const info, uint32_t const *const value)
{
    ASSERT(info->action == CCAPI_RCI_ACTION_SET);

    ASSERT(info->query_setting.attributes.compare_to == CCAPI_RCI_QUERY_SETTING_ATTRIBUTE_COMPARE_TO_NONE);
    ASSERT(info->query_setting.attributes.source == CCAPI_RCI_QUERY_SETTING_ATTRIBUTE_SOURCE_CURRENT);
    ASSERT(info->group.type == CCAPI_RCI_GROUP_SETTING);
    ASSERT(strcmp(info->group.name, "group_1") == 0);
    ASSERT(strcmp(info->element.name, "el_uint32") == 0);
    ASSERT(info->query_setting.matches == CCAPI_FALSE);
    ASSERT(info->error_hint == NULL);
    ASSERT(info->user_context == NULL);

    UNUSED_PARAMETER(value);
    return CCAPI_GLOBAL_ERROR_NONE;
}

ccapi_setting_group_1_error_id_t rci_setting_group_1_el_on_off_get(ccapi_rci_info_t * const info, ccapi_on_off_t * const value)
{
    ASSERT(info->action == CCAPI_RCI_ACTION_QUERY);

    printf("%s: info->query_setting.attributes.compare_to %d\n", __FUNCTION__, info->query_setting.attributes.compare_to);
    printf("%s: info->query_setting.attributes.source %d\n", __FUNCTION__, info->query_setting.attributes.source);
    printf("%s: info->group.instance %d\n", __FUNCTION__, info->group.instance);
    ASSERT(info->group.type == CCAPI_RCI_GROUP_SETTING);
    ASSERT(strcmp(info->group.name, "group_1") == 0);
    ASSERT(strcmp(info->element.name, "el_on_off") == 0);
    ASSERT(info->query_setting.matches == CCAPI_FALSE);
    ASSERT(info->error_hint == NULL);
    ASSERT(info->user_context == NULL);

    *value = CCAPI_ON;
    return CCAPI_GLOBAL_ERROR_NONE;
}

ccapi_setting_group_1_error_id_t rci_setting_group_1_el_on_off_set(ccapi_rci_info_t * const info, ccapi_on_off_t const *const value)
{
    ASSERT(info->action == CCAPI_RCI_ACTION_SET);

    ASSERT(info->query_setting.attributes.compare_to == CCAPI_RCI_QUERY_SETTING_ATTRIBUTE_COMPARE_TO_NONE);
    ASSERT(info->query_setting.attributes.source == CCAPI_RCI_QUERY_SETTING_ATTRIBUTE_SOURCE_CURRENT);
    ASSERT(info->group.type == CCAPI_RCI_GROUP_SETTING);
    ASSERT(strcmp(info->group.name, "group_1") == 0);
    ASSERT(strcmp(info->element.name, "el_on_off") == 0);
    ASSERT(info->query_setting.matches == CCAPI_FALSE);
    ASSERT(info->error_hint == NULL);
    ASSERT(info->user_context == NULL);

    UNUSED_PARAMETER(value);
    return CCAPI_GLOBAL_ERROR_NONE;
}

ccapi_setting_group_1_error_id_t rci_setting_group_1_el_hex_get(ccapi_rci_info_t * const info, uint32_t * const value)
{
    ASSERT(info->action == CCAPI_RCI_ACTION_QUERY);

    printf("%s: info->query_setting.attributes.compare_to %d\n", __FUNCTION__, info->query_setting.attributes.compare_to);
    printf("%s: info->query_setting.attributes.source %d\n", __FUNCTION__, info->query_setting.attributes.source);
    printf("%s: info->group.instance %d\n", __FUNCTION__, info->group.instance);

    ASSERT(info->query_setting.attributes.compare_to == CCAPI_RCI_QUERY_SETTING_ATTRIBUTE_COMPARE_TO_NONE);
    ASSERT(info->query_setting.attributes.source == CCAPI_RCI_QUERY_SETTING_ATTRIBUTE_SOURCE_CURRENT);
    ASSERT(info->group.type == CCAPI_RCI_GROUP_SETTING);
    ASSERT(strcmp(info->group.name, "group_1") == 0);
    ASSERT(strcmp(info->element.name, "el_hex") == 0);
    ASSERT(info->query_setting.matches == CCAPI_FALSE);
    ASSERT(info->error_hint == NULL);
    ASSERT(info->user_context == NULL);

    *value = 0x20101010;
    return CCAPI_GLOBAL_ERROR_NONE;
}

ccapi_setting_group_1_error_id_t rci_setting_group_1_el_hex_set(ccapi_rci_info_t * const info, uint32_t const *const value)
{
    ASSERT(info->action == CCAPI_RCI_ACTION_SET);

    ASSERT(info->query_setting.attributes.compare_to == CCAPI_RCI_QUERY_SETTING_ATTRIBUTE_COMPARE_TO_NONE);
    ASSERT(info->query_setting.attributes.source == CCAPI_RCI_QUERY_SETTING_ATTRIBUTE_SOURCE_CURRENT);
    ASSERT(info->group.type == CCAPI_RCI_GROUP_SETTING);
    ASSERT(strcmp(info->group.name, "group_1") == 0);
    ASSERT(strcmp(info->element.name, "el_hex") == 0);
    ASSERT(info->query_setting.matches == CCAPI_FALSE);
    ASSERT(info->error_hint == NULL);
    ASSERT(info->user_context == NULL);

    UNUSED_PARAMETER(value);
    return CCAPI_GLOBAL_ERROR_NONE;
}

ccapi_setting_group_1_error_id_t rci_setting_group_1_el_0xhex_get(ccapi_rci_info_t * const info, uint32_t * const value)
{
    ASSERT(info->action == CCAPI_RCI_ACTION_QUERY);

    printf("%s: info->query_setting.attributes.compare_to %d\n", __FUNCTION__, info->query_setting.attributes.compare_to);
    printf("%s: info->query_setting.attributes.source %d\n", __FUNCTION__, info->query_setting.attributes.source);
    printf("%s: info->group.instance %d\n", __FUNCTION__, info->group.instance);

    ASSERT(info->group.type == CCAPI_RCI_GROUP_SETTING);
    ASSERT(strcmp(info->group.name, "group_1") == 0);
    ASSERT(strcmp(info->element.name, "el_0xhex") == 0);
    ASSERT(info->query_setting.matches == CCAPI_FALSE);
    ASSERT(info->error_hint == NULL);
    ASSERT(info->user_context == NULL);

    *value = 0x20101010;
    return CCAPI_GLOBAL_ERROR_NONE;
}

ccapi_setting_group_1_error_id_t rci_setting_group_1_el_0xhex_set(ccapi_rci_info_t * const info, uint32_t const *const value)
{
    ASSERT(info->action == CCAPI_RCI_ACTION_SET);

    ASSERT(info->query_setting.attributes.compare_to == CCAPI_RCI_QUERY_SETTING_ATTRIBUTE_COMPARE_TO_NONE);
    ASSERT(info->query_setting.attributes.source == CCAPI_RCI_QUERY_SETTING_ATTRIBUTE_SOURCE_CURRENT);
    ASSERT(info->group.type == CCAPI_RCI_GROUP_SETTING);
    ASSERT(strcmp(info->group.name, "group_1") == 0);
    ASSERT(strcmp(info->element.name, "el_0xhex") == 0);
    ASSERT(info->query_setting.matches == CCAPI_FALSE);
    ASSERT(info->error_hint == NULL);
    ASSERT(info->user_context == NULL);

    UNUSED_PARAMETER(value);
    return CCAPI_GLOBAL_ERROR_NONE;
}

ccapi_setting_group_1_error_id_t rci_setting_group_1_el_signed_get(ccapi_rci_info_t * const info, int32_t * const value)
{
    ASSERT(info->action == CCAPI_RCI_ACTION_QUERY);

    printf("%s: info->query_setting.attributes.compare_to %d\n", __FUNCTION__, info->query_setting.attributes.compare_to);
    printf("%s: info->query_setting.attributes.source %d\n", __FUNCTION__, info->query_setting.attributes.source);
    printf("%s: info->group.instance %d\n", __FUNCTION__, info->group.instance);

    ASSERT(info->group.type == CCAPI_RCI_GROUP_SETTING);
    ASSERT(strcmp(info->group.name, "group_1") == 0);
    ASSERT(strcmp(info->element.name, "el_signed") == 0);
    ASSERT(info->query_setting.matches == CCAPI_FALSE);
    ASSERT(info->error_hint == NULL);
    ASSERT(info->user_context == NULL);

    *value = -100;
    return CCAPI_GLOBAL_ERROR_NONE;
}

ccapi_setting_group_1_error_id_t rci_setting_group_1_el_signed_set(ccapi_rci_info_t * const info, int32_t const *const value)
{
    ASSERT(info->action == CCAPI_RCI_ACTION_SET);

    ASSERT(info->query_setting.attributes.compare_to == CCAPI_RCI_QUERY_SETTING_ATTRIBUTE_COMPARE_TO_NONE);
    ASSERT(info->query_setting.attributes.source == CCAPI_RCI_QUERY_SETTING_ATTRIBUTE_SOURCE_CURRENT);
    ASSERT(info->group.type == CCAPI_RCI_GROUP_SETTING);
    ASSERT(strcmp(info->group.name, "group_1") == 0);
    ASSERT(strcmp(info->element.name, "el_signed") == 0);
    ASSERT(info->query_setting.matches == CCAPI_FALSE);
    ASSERT(info->error_hint == NULL);
    ASSERT(info->user_context == NULL);

    UNUSED_PARAMETER(value);
    return CCAPI_GLOBAL_ERROR_NONE;
}

ccapi_setting_group_1_error_id_t rci_setting_group_1_el_bool_get(ccapi_rci_info_t * const info, ccapi_bool_t * const value)
{
    ASSERT(info->action == CCAPI_RCI_ACTION_QUERY);

    printf("%s: info->query_setting.attributes.compare_to %d\n", __FUNCTION__, info->query_setting.attributes.compare_to);
    printf("%s: info->query_setting.attributes.source %d\n", __FUNCTION__, info->query_setting.attributes.source);
    printf("%s: info->group.instance %d\n", __FUNCTION__, info->group.instance);

    ASSERT(info->group.type == CCAPI_RCI_GROUP_SETTING);
    ASSERT(strcmp(info->group.name, "group_1") == 0);
    ASSERT(strcmp(info->element.name, "el_bool") == 0);
    ASSERT(info->query_setting.matches == CCAPI_FALSE);
    ASSERT(info->error_hint == NULL);
    ASSERT(info->user_context == NULL);

    *value = CCAPI_TRUE;
    return CCAPI_GLOBAL_ERROR_NONE;
}

ccapi_setting_group_1_error_id_t rci_setting_group_1_el_bool_set(ccapi_rci_info_t * const info, ccapi_bool_t const *const value)
{
    ASSERT(info->action == CCAPI_RCI_ACTION_SET);

    ASSERT(info->query_setting.attributes.compare_to == CCAPI_RCI_QUERY_SETTING_ATTRIBUTE_COMPARE_TO_NONE);
    ASSERT(info->query_setting.attributes.source == CCAPI_RCI_QUERY_SETTING_ATTRIBUTE_SOURCE_CURRENT);
    ASSERT(info->group.type == CCAPI_RCI_GROUP_SETTING);
    ASSERT(strcmp(info->group.name, "group_1") == 0);
    ASSERT(strcmp(info->element.name, "el_bool") == 0);
    ASSERT(info->query_setting.matches == CCAPI_FALSE);
    ASSERT(info->error_hint == NULL);
    ASSERT(info->user_context == NULL);

    UNUSED_PARAMETER(value);
    return CCAPI_GLOBAL_ERROR_NONE;
}

ccapi_setting_group_1_error_id_t rci_setting_group_1_el_float_get(ccapi_rci_info_t * const info, float * const value)
{
    ASSERT(info->action == CCAPI_RCI_ACTION_QUERY);

    printf("%s: info->query_setting.attributes.compare_to %d\n", __FUNCTION__, info->query_setting.attributes.compare_to);
    printf("%s: info->query_setting.attributes.source %d\n", __FUNCTION__, info->query_setting.attributes.source);
    printf("%s: info->group.instance %d\n", __FUNCTION__, info->group.instance);

    ASSERT(info->group.type == CCAPI_RCI_GROUP_SETTING);
    ASSERT(strcmp(info->group.name, "group_1") == 0);
    ASSERT(strcmp(info->element.name, "el_float") == 0);
    ASSERT(info->query_setting.matches == CCAPI_FALSE);
    ASSERT(info->error_hint == NULL);
    ASSERT(info->user_context == NULL);

    *value = 1.2;
    return CCAPI_GLOBAL_ERROR_NONE;
}

ccapi_setting_group_1_error_id_t rci_setting_group_1_el_float_set(ccapi_rci_info_t * const info, float const *const value)
{
    ASSERT(info->action == CCAPI_RCI_ACTION_SET);

    ASSERT(info->query_setting.attributes.compare_to == CCAPI_RCI_QUERY_SETTING_ATTRIBUTE_COMPARE_TO_NONE);
    ASSERT(info->query_setting.attributes.source == CCAPI_RCI_QUERY_SETTING_ATTRIBUTE_SOURCE_CURRENT);
    ASSERT(info->group.type == CCAPI_RCI_GROUP_SETTING);
    ASSERT(strcmp(info->group.name, "group_1") == 0);
    ASSERT(strcmp(info->element.name, "el_float") == 0);
    ASSERT(info->query_setting.matches == CCAPI_FALSE);
    ASSERT(info->error_hint == NULL);
    ASSERT(info->user_context == NULL);

    UNUSED_PARAMETER(value);
    return CCAPI_GLOBAL_ERROR_NONE;
}

ccapi_setting_group_3_error_id_t rci_setting_group_3_start(ccapi_rci_info_t * const info)
{
    printf("%s: info->action %d\n", __FUNCTION__, info->action);

    if (info->action == CCAPI_RCI_ACTION_QUERY && info->group.type == CCAPI_RCI_GROUP_SETTING)
    {
        printf("%s: info->query_setting.attributes.compare_to %d\n", __FUNCTION__, info->query_setting.attributes.compare_to);
        printf("%s: info->query_setting.attributes.source %d\n", __FUNCTION__, info->query_setting.attributes.source);
    }
    else
    {
        ASSERT(info->query_setting.attributes.compare_to == CCAPI_RCI_QUERY_SETTING_ATTRIBUTE_COMPARE_TO_NONE);
        ASSERT(info->query_setting.attributes.source == CCAPI_RCI_QUERY_SETTING_ATTRIBUTE_SOURCE_CURRENT);
    }
    printf("%s: info->group.instance %d\n", __FUNCTION__, info->group.instance);
    ASSERT(info->group.type == CCAPI_RCI_GROUP_SETTING);
    ASSERT(strcmp(info->group.name, "group_3") == 0);
    ASSERT(info->element.name == NULL);
    ASSERT(info->query_setting.matches == CCAPI_FALSE);
    ASSERT(info->error_hint == NULL);
    ASSERT(info->user_context == NULL);

    return CCAPI_GLOBAL_ERROR_NONE;
}

ccapi_setting_group_3_error_id_t rci_setting_group_3_end(ccapi_rci_info_t * const info)
{
    printf("%s: info->type %d\n", __FUNCTION__, info->group.type);

    if (info->action == CCAPI_RCI_ACTION_QUERY && info->group.type == CCAPI_RCI_GROUP_SETTING)
    {
        printf("%s: info->query_setting.attributes.compare_to %d\n", __FUNCTION__, info->query_setting.attributes.compare_to);
        printf("%s: info->query_setting.attributes.source %d\n", __FUNCTION__, info->query_setting.attributes.source);
    }
    else
    {
        ASSERT(info->query_setting.attributes.compare_to == CCAPI_RCI_QUERY_SETTING_ATTRIBUTE_COMPARE_TO_NONE);
        ASSERT(info->query_setting.attributes.source == CCAPI_RCI_QUERY_SETTING_ATTRIBUTE_SOURCE_CURRENT);
    }
    printf("%s: info->group.instance %d\n", __FUNCTION__, info->group.instance);
    ASSERT(info->group.type == CCAPI_RCI_GROUP_SETTING);
    ASSERT(strcmp(info->group.name, "group_3") == 0);
    ASSERT(info->query_setting.matches == CCAPI_FALSE);
    ASSERT(info->error_hint == NULL);
    ASSERT(info->user_context == NULL);

    return CCAPI_GLOBAL_ERROR_NONE;
}

ccapi_setting_group_3_error_id_t rci_setting_group_3_el_string_get(ccapi_rci_info_t * const info, char const * * const value)
{
    ASSERT(info->action == CCAPI_RCI_ACTION_QUERY);

    printf("%s: info->query_setting.attributes.compare_to %d\n", __FUNCTION__, info->query_setting.attributes.compare_to);
    printf("%s: info->query_setting.attributes.source %d\n", __FUNCTION__, info->query_setting.attributes.source);
    printf("%s: info->group.instance %d\n", __FUNCTION__, info->group.instance);

    ASSERT(info->group.type == CCAPI_RCI_GROUP_SETTING);
    ASSERT(strcmp(info->group.name, "group_3") == 0);
    ASSERT(strcmp(info->element.name, "el_string") == 0);
    ASSERT(info->query_setting.matches == CCAPI_FALSE);
    ASSERT(info->error_hint == NULL);
    ASSERT(info->user_context == NULL);

    *value = "String";
    return CCAPI_GLOBAL_ERROR_NONE;
}

ccapi_setting_group_3_error_id_t rci_setting_group_3_el_string_set(ccapi_rci_info_t * const info, char const * const value)
{
    ASSERT(info->action == CCAPI_RCI_ACTION_SET);

    ASSERT(info->query_setting.attributes.compare_to == CCAPI_RCI_QUERY_SETTING_ATTRIBUTE_COMPARE_TO_NONE);
    ASSERT(info->query_setting.attributes.source == CCAPI_RCI_QUERY_SETTING_ATTRIBUTE_SOURCE_CURRENT);
    ASSERT(info->group.type == CCAPI_RCI_GROUP_SETTING);
    ASSERT(strcmp(info->group.name, "group_3") == 0);
    ASSERT(strcmp(info->element.name, "el_string") == 0);
    ASSERT(info->query_setting.matches == CCAPI_FALSE);
    ASSERT(info->error_hint == NULL);
    ASSERT(info->user_context == NULL);

    UNUSED_PARAMETER(value);
    return CCAPI_GLOBAL_ERROR_NONE;
}

ccapi_setting_group_3_error_id_t rci_setting_group_3_el_multiline_get(ccapi_rci_info_t * const info, char const * * const value)
{
    ASSERT(info->action == CCAPI_RCI_ACTION_QUERY);

    printf("%s: info->query_setting.attributes.compare_to %d\n", __FUNCTION__, info->query_setting.attributes.compare_to);
    printf("%s: info->query_setting.attributes.source %d\n", __FUNCTION__, info->query_setting.attributes.source);
    printf("%s: info->group.instance %d\n", __FUNCTION__, info->group.instance);

    ASSERT(info->group.type == CCAPI_RCI_GROUP_SETTING);
    ASSERT(strcmp(info->group.name, "group_3") == 0);
    ASSERT(strcmp(info->element.name, "el_multiline") == 0);
    ASSERT(info->query_setting.matches == CCAPI_FALSE);
    ASSERT(info->error_hint == NULL);
    ASSERT(info->user_context == NULL);

    *value = "String";
    return CCAPI_GLOBAL_ERROR_NONE;
}

ccapi_setting_group_3_error_id_t rci_setting_group_3_el_multiline_set(ccapi_rci_info_t * const info, char const * const value)
{
    ASSERT(info->action == CCAPI_RCI_ACTION_SET);

    ASSERT(info->query_setting.attributes.compare_to == CCAPI_RCI_QUERY_SETTING_ATTRIBUTE_COMPARE_TO_NONE);
    ASSERT(info->query_setting.attributes.source == CCAPI_RCI_QUERY_SETTING_ATTRIBUTE_SOURCE_CURRENT);
    ASSERT(info->group.type == CCAPI_RCI_GROUP_SETTING);
    ASSERT(strcmp(info->group.name, "group_3") == 0);
    ASSERT(strcmp(info->element.name, "el_multiline") == 0);
    ASSERT(info->query_setting.matches == CCAPI_FALSE);
    ASSERT(info->error_hint == NULL);
    ASSERT(info->user_context == NULL);

    UNUSED_PARAMETER(value);
    return CCAPI_GLOBAL_ERROR_NONE;
}

ccapi_setting_group_3_error_id_t rci_setting_group_3_el_password_set(ccapi_rci_info_t * const info, char const * const value)
{
    ASSERT(info->action == CCAPI_RCI_ACTION_SET);

    ASSERT(info->query_setting.attributes.compare_to == CCAPI_RCI_QUERY_SETTING_ATTRIBUTE_COMPARE_TO_NONE);
    ASSERT(info->query_setting.attributes.source == CCAPI_RCI_QUERY_SETTING_ATTRIBUTE_SOURCE_CURRENT);
    ASSERT(info->group.type == CCAPI_RCI_GROUP_SETTING);
    ASSERT(strcmp(info->group.name, "group_3") == 0);
    ASSERT(strcmp(info->element.name, "el_password") == 0);
    ASSERT(info->query_setting.matches == CCAPI_FALSE);
    ASSERT(info->error_hint == NULL);
    ASSERT(info->user_context == NULL);

    UNUSED_PARAMETER(value);
    return CCAPI_GLOBAL_ERROR_NONE;
}

ccapi_state_group_2_error_id_t rci_state_group_2_start(ccapi_rci_info_t * const info)
{
    printf("%s: info->action %d\n", __FUNCTION__, info->action);

    if (info->action == CCAPI_RCI_ACTION_QUERY && info->group.type == CCAPI_RCI_GROUP_SETTING)
    {
        printf("%s: info->query_setting.attributes.compare_to %d\n", __FUNCTION__, info->query_setting.attributes.compare_to);
        printf("%s: info->query_setting.attributes.source %d\n", __FUNCTION__, info->query_setting.attributes.source);
    }
    else
    {
        ASSERT(info->query_setting.attributes.compare_to == CCAPI_RCI_QUERY_SETTING_ATTRIBUTE_COMPARE_TO_NONE);
        ASSERT(info->query_setting.attributes.source == CCAPI_RCI_QUERY_SETTING_ATTRIBUTE_SOURCE_CURRENT);
    }
    printf("%s: info->group.instance %d\n", __FUNCTION__, info->group.instance);
    ASSERT(info->group.type == CCAPI_RCI_GROUP_STATE);
    ASSERT(strcmp(info->group.name, "group_2") == 0);
    ASSERT(info->element.name == NULL);
    ASSERT(info->query_setting.matches == CCAPI_FALSE);
    ASSERT(info->error_hint == NULL);
    ASSERT(info->user_context == NULL);

    return CCAPI_GLOBAL_ERROR_NONE;
}

ccapi_state_group_2_error_id_t rci_state_group_2_end(ccapi_rci_info_t * const info)
{
    printf("%s: info->action %d\n", __FUNCTION__, info->action);

    if (info->action == CCAPI_RCI_ACTION_QUERY && info->group.type == CCAPI_RCI_GROUP_SETTING)
    {
        printf("%s: info->query_setting.attributes.compare_to %d\n", __FUNCTION__, info->query_setting.attributes.compare_to);
        printf("%s: info->query_setting.attributes.source %d\n", __FUNCTION__, info->query_setting.attributes.source);
    }
    else
    {
        ASSERT(info->query_setting.attributes.compare_to == CCAPI_RCI_QUERY_SETTING_ATTRIBUTE_COMPARE_TO_NONE);
        ASSERT(info->query_setting.attributes.source == CCAPI_RCI_QUERY_SETTING_ATTRIBUTE_SOURCE_CURRENT);
    }
    printf("%s: info->group.instance %d\n", __FUNCTION__, info->group.instance);
    ASSERT(info->group.type == CCAPI_RCI_GROUP_STATE);
    ASSERT(strcmp(info->group.name, "group_2") == 0);
    ASSERT(info->query_setting.matches == CCAPI_FALSE);
    ASSERT(info->error_hint == NULL);
    ASSERT(info->user_context == NULL);

    return CCAPI_GLOBAL_ERROR_NONE;
}

ccapi_state_group_2_error_id_t rci_state_group_2_el_ip_get(ccapi_rci_info_t * const info, char const * * const value)
{
    ASSERT(info->action == CCAPI_RCI_ACTION_QUERY);

    printf("%s: info->query_setting.attributes.compare_to %d\n", __FUNCTION__, info->query_setting.attributes.compare_to);
    printf("%s: info->query_setting.attributes.source %d\n", __FUNCTION__, info->query_setting.attributes.source);
    printf("%s: info->group.instance %d\n", __FUNCTION__, info->group.instance);

    ASSERT(info->group.type == CCAPI_RCI_GROUP_STATE);
    ASSERT(strcmp(info->group.name, "group_2") == 0);
    ASSERT(strcmp(info->element.name, "el_ip") == 0);
    ASSERT(info->query_setting.matches == CCAPI_FALSE);
    ASSERT(info->error_hint == NULL);
    ASSERT(info->user_context == NULL);

    *value = "192.168.1.1";
    return CCAPI_GLOBAL_ERROR_NONE;
}

ccapi_state_group_2_error_id_t rci_state_group_2_el_ip_set(ccapi_rci_info_t * const info, char const * const value)
{
    ASSERT(info->action == CCAPI_RCI_ACTION_SET);

    ASSERT(info->query_setting.attributes.compare_to == CCAPI_RCI_QUERY_SETTING_ATTRIBUTE_COMPARE_TO_NONE);
    ASSERT(info->query_setting.attributes.source == CCAPI_RCI_QUERY_SETTING_ATTRIBUTE_SOURCE_CURRENT);
    ASSERT(info->group.type == CCAPI_RCI_GROUP_STATE);
    ASSERT(strcmp(info->group.name, "group_2") == 0);
    ASSERT(strcmp(info->element.name, "el_ip") == 0);
    ASSERT(info->query_setting.matches == CCAPI_FALSE);
    ASSERT(info->error_hint == NULL);
    ASSERT(info->user_context == NULL);

    UNUSED_PARAMETER(value);
    return CCAPI_GLOBAL_ERROR_NONE;
}

ccapi_state_group_2_error_id_t rci_state_group_2_el_fqdnv4_get(ccapi_rci_info_t * const info, char const * * const value)
{
    ASSERT(info->action == CCAPI_RCI_ACTION_QUERY);

    printf("%s: info->query_setting.attributes.compare_to %d\n", __FUNCTION__, info->query_setting.attributes.compare_to);
    printf("%s: info->query_setting.attributes.source %d\n", __FUNCTION__, info->query_setting.attributes.source);
    printf("%s: info->group.instance %d\n", __FUNCTION__, info->group.instance);

    ASSERT(info->group.type == CCAPI_RCI_GROUP_STATE);
    ASSERT(strcmp(info->group.name, "group_2") == 0);
    ASSERT(strcmp(info->element.name, "el_fqdnv4") == 0);
    ASSERT(info->query_setting.matches == CCAPI_FALSE);
    ASSERT(info->error_hint == NULL);
    ASSERT(info->user_context == NULL);

    *value = "192.168.1.1";
    return CCAPI_GLOBAL_ERROR_NONE;
}

ccapi_state_group_2_error_id_t rci_state_group_2_el_fqdnv4_set(ccapi_rci_info_t * const info, char const * const value)
{
    ASSERT(info->action == CCAPI_RCI_ACTION_SET);

    ASSERT(info->query_setting.attributes.compare_to == CCAPI_RCI_QUERY_SETTING_ATTRIBUTE_COMPARE_TO_NONE);
    ASSERT(info->query_setting.attributes.source == CCAPI_RCI_QUERY_SETTING_ATTRIBUTE_SOURCE_CURRENT);
    ASSERT(info->group.type == CCAPI_RCI_GROUP_STATE);
    ASSERT(strcmp(info->group.name, "group_2") == 0);
    ASSERT(strcmp(info->element.name, "el_fqdnv4") == 0);
    ASSERT(info->query_setting.matches == CCAPI_FALSE);
    ASSERT(info->error_hint == NULL);
    ASSERT(info->user_context == NULL);

    UNUSED_PARAMETER(value);
    return CCAPI_GLOBAL_ERROR_NONE;
}

ccapi_state_group_2_error_id_t rci_state_group_2_el_fqdnv6_get(ccapi_rci_info_t * const info, char const * * const value)
{
    ASSERT(info->action == CCAPI_RCI_ACTION_QUERY);

    printf("%s: info->query_setting.attributes.compare_to %d\n", __FUNCTION__, info->query_setting.attributes.compare_to);
    printf("%s: info->query_setting.attributes.source %d\n", __FUNCTION__, info->query_setting.attributes.source);
    printf("%s: info->group.instance %d\n", __FUNCTION__, info->group.instance);

    ASSERT(info->group.type == CCAPI_RCI_GROUP_STATE);
    ASSERT(strcmp(info->group.name, "group_2") == 0);
    ASSERT(strcmp(info->element.name, "el_fqdnv6") == 0);
    ASSERT(info->query_setting.matches == CCAPI_FALSE);
    ASSERT(info->error_hint == NULL);
    ASSERT(info->user_context == NULL);

    *value = "192.168.1.1";
    return CCAPI_GLOBAL_ERROR_NONE;
}

ccapi_state_group_2_error_id_t rci_state_group_2_el_fqdnv6_set(ccapi_rci_info_t * const info, char const * const value)
{
    ASSERT(info->action == CCAPI_RCI_ACTION_SET);

    ASSERT(info->query_setting.attributes.compare_to == CCAPI_RCI_QUERY_SETTING_ATTRIBUTE_COMPARE_TO_NONE);
    ASSERT(info->query_setting.attributes.source == CCAPI_RCI_QUERY_SETTING_ATTRIBUTE_SOURCE_CURRENT);
    ASSERT(info->group.type == CCAPI_RCI_GROUP_STATE);
    ASSERT(strcmp(info->group.name, "group_1") == 0);
    ASSERT(strcmp(info->element.name, "el_fqdnv6") == 0);
    ASSERT(info->query_setting.matches == CCAPI_FALSE);
    ASSERT(info->error_hint == NULL);
    ASSERT(info->user_context == NULL);

    UNUSED_PARAMETER(value);
    return CCAPI_GLOBAL_ERROR_NONE;
}

ccapi_state_group_2_error_id_t rci_state_group_2_el_mac_get(ccapi_rci_info_t * const info, char const * * const value)
{
    ASSERT(info->action == CCAPI_RCI_ACTION_QUERY);

    printf("%s: info->query_setting.attributes.compare_to %d\n", __FUNCTION__, info->query_setting.attributes.compare_to);
    printf("%s: info->query_setting.attributes.source %d\n", __FUNCTION__, info->query_setting.attributes.source);
    printf("%s: info->group.instance %d\n", __FUNCTION__, info->group.instance);

    ASSERT(info->group.type == CCAPI_RCI_GROUP_STATE);
    ASSERT(strcmp(info->group.name, "group_2") == 0);
    ASSERT(strcmp(info->element.name, "el_mac") == 0);
    ASSERT(info->query_setting.matches == CCAPI_FALSE);
    ASSERT(info->error_hint == NULL);
    ASSERT(info->user_context == NULL);

    *value = "00:04:9D:AB:CD:EF";
    return CCAPI_GLOBAL_ERROR_NONE;
}

ccapi_state_group_2_error_id_t rci_state_group_2_el_mac_set(ccapi_rci_info_t * const info, char const * const value)
{
    ASSERT(info->action == CCAPI_RCI_ACTION_SET);

    ASSERT(info->query_setting.attributes.compare_to == CCAPI_RCI_QUERY_SETTING_ATTRIBUTE_COMPARE_TO_NONE);
    ASSERT(info->query_setting.attributes.source == CCAPI_RCI_QUERY_SETTING_ATTRIBUTE_SOURCE_CURRENT);
    ASSERT(info->group.type == CCAPI_RCI_GROUP_STATE);
    ASSERT(strcmp(info->group.name, "group_2") == 0);
    ASSERT(strcmp(info->element.name, "el_mac") == 0);
    ASSERT(info->query_setting.matches == CCAPI_FALSE);
    ASSERT(info->error_hint == NULL);
    ASSERT(info->user_context == NULL);

    UNUSED_PARAMETER(value);
    return CCAPI_GLOBAL_ERROR_NONE;
}

ccapi_state_group_2_error_id_t rci_state_group_2_el_datetime_get(ccapi_rci_info_t * const info, char const * * const value)
{
    ASSERT(info->action == CCAPI_RCI_ACTION_QUERY);

    printf("%s: info->query_setting.attributes.compare_to %d\n", __FUNCTION__, info->query_setting.attributes.compare_to);
    printf("%s: info->query_setting.attributes.source %d\n", __FUNCTION__, info->query_setting.attributes.source);
    printf("%s: info->group.instance %d\n", __FUNCTION__, info->group.instance);

    ASSERT(info->group.type == CCAPI_RCI_GROUP_STATE);
    ASSERT(strcmp(info->group.name, "group_2") == 0);
    ASSERT(strcmp(info->element.name, "el_datetime") == 0);
    ASSERT(info->query_setting.matches == CCAPI_FALSE);
    ASSERT(info->error_hint == NULL);
    ASSERT(info->user_context == NULL);

    *value = "2002-05-30T09:30:10-0600";;
    return CCAPI_GLOBAL_ERROR_NONE;
}

ccapi_state_group_2_error_id_t rci_state_group_2_el_datetime_set(ccapi_rci_info_t * const info, char const * const value)
{
    ASSERT(info->action == CCAPI_RCI_ACTION_SET);

    ASSERT(info->query_setting.attributes.compare_to == CCAPI_RCI_QUERY_SETTING_ATTRIBUTE_COMPARE_TO_NONE);
    ASSERT(info->query_setting.attributes.source == CCAPI_RCI_QUERY_SETTING_ATTRIBUTE_SOURCE_CURRENT);
    ASSERT(info->group.type == CCAPI_RCI_GROUP_STATE);
    ASSERT(strcmp(info->group.name, "group_2") == 0);
    ASSERT(strcmp(info->element.name, "el_datetime") == 0);
    ASSERT(info->query_setting.matches == CCAPI_FALSE);
    ASSERT(info->error_hint == NULL);
    ASSERT(info->user_context == NULL);

    UNUSED_PARAMETER(value);
    return CCAPI_GLOBAL_ERROR_NONE;
}
