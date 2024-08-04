#ifndef FAKE_REGISTRY_HEADER
#define FAKE_REGISTRY_HEADER

#include "types.h"
#include "Windows.h"

void InitializeFakeRegistry();
BOOL SaveFakeRegistry();

BOOL GetFakeRegistryItem_String(const char *key_ptr, char **value_ptr);
BOOL SetFakeRegistryItem_String(const char *key_ptr, const char *value_ptr);

BOOL GetFakeRegistryItem_Double(const char *key_ptr, double *double_ptr);
BOOL SetFakeRegistryItem_Double(const char *key_ptr, double double_val);

BOOL GetFakeRegistryItem_Long(const char *key_ptr, long *long_val);
BOOL SetFakeRegistryItem_Long(const char *key_ptr, long long_val);

void RemoveFakeRegistryItem(const char *key_ptr);

void DestroyFakeRegistry();

void DumpFakeRegistry();

#endif
