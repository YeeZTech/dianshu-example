#include "../enclave/user_type.h"
#include <toolkit/plugins/csv/csv_reader.h>
typedef ypc::plugins::typed_csv_reader<user_item_t> user_reader_t;
impl_csv_reader(user_reader_t)
