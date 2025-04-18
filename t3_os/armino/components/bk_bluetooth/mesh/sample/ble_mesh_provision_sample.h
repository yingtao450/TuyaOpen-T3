#pragma once

#define PROVISION_TYPE_WHEN_INIT 1
#define PROVISION_TYPE_MANUAL 2
#define PROVISION_TYPE_AUTO 3

//#define PROVISION_TYPE PROVISION_TYPE_WHEN_INIT



#define BEKEN_VND_COMPANY_ID 0x05f0
#define BEKEN_VND_MODEL_COUNT_CLIENT 0xff01
#define BEKEN_VND_MODEL_COUNT_SERVER 0xff02


enum
{
    PROVISION_ROLE_UNKNOW,
    PROVISION_ROLE_PROVISIONER,
    PROVISION_ROLE_PROVISIONEE,
};

#define BEKEN_VND_PROVISIONER_UUID_HEAD BEKEN_VND_COMPANY_ID & 0xff, (BEKEN_VND_COMPANY_ID >> 8) & 0xff, 0x00 + 1, 0xfe
#define BEKEN_VND_PROVISIONEE_UUID_HEAD BEKEN_VND_COMPANY_ID & 0xff, (BEKEN_VND_COMPANY_ID >> 8) & 0xff, 0x00, 0xfe

enum
{
    PROVISION_STATUS_IDLE,
    PROVISION_STATUS_PROVISIONING,
    PROVISION_STATUS_WAIT_CLOSE,
    PROVISION_STATUS_PROVISION_COMPL,
};

int bt_mesh_provision_sample_init(void);
void bt_mesh_provision_sample_shell(int32_t argc, char **argv);

