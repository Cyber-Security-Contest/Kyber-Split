#include <tee_internal_api.h>
#include <tee_internal_api_extensions.h>
#include "kyber_split_ta.h"

TEE_Result secure_add(uint32_t a, uint32_t b, uint32_t *out)
{
    *out = a + b;
    return TEE_SUCCESS;
}
