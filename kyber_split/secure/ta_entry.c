#include <tee_internal_api.h>
#include <tee_internal_api_extensions.h>
#include "kyber_split_ta.h"

TEE_Result TA_CreateEntryPoint(void) { return TEE_SUCCESS; }
void TA_DestroyEntryPoint(void) {}

TEE_Result TA_OpenSessionEntryPoint(uint32_t param_types,
                                    TEE_Param params[4],
                                    void **session_context)
{
    return TEE_SUCCESS;
}

void TA_CloseSessionEntryPoint(void *session_context) {}

extern TEE_Result secure_add(uint32_t a, uint32_t b, uint32_t *out);

TEE_Result TA_InvokeCommandEntryPoint(void *sess_ctx,
                                      uint32_t cmd_id,
                                      uint32_t param_types,
                                      TEE_Param params[4])
{
    switch (cmd_id) {
    case TA_SECURE_ADD:
        return secure_add(params[0].value.a,
                          params[0].value.b,
                          &params[1].value.a);
    default:
        return TEE_ERROR_NOT_SUPPORTED;
    }
}
