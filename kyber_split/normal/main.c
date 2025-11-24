#include <stdio.h>
#include <tee_client_api.h>
#include "kyber_split_ca.h"

int main()
{
    TEEC_Context ctx;
    TEEC_Session sess;
    TEEC_Operation op;
    TEEC_UUID uuid = TA_KYBER_SPLIT_UUID;
    uint32_t err_origin;

    uint32_t a = 5, b = 7, result = 0;

    TEEC_InitializeContext(NULL, &ctx);
    TEEC_OpenSession(&ctx, &sess, &uuid, TEEC_LOGIN_PUBLIC,
                     NULL, NULL, &err_origin);

    memset(&op, 0, sizeof(op));
    op.paramTypes = TEEC_PARAM_TYPES(TEEC_VALUE_INOUT,
                                     TEEC_VALUE_OUTPUT,
                                     TEEC_NONE, TEEC_NONE);

    op.params[0].value.a = a;
    op.params[0].value.b = b;

    TEEC_InvokeCommand(&sess, TA_SECURE_ADD, &op, &err_origin);

    result = op.params[1].value.a;
    printf("Secure World returned: %d\n", result);

    TEEC_CloseSession(&sess);
    TEEC_FinalizeContext(&ctx);

    return 0;
}
