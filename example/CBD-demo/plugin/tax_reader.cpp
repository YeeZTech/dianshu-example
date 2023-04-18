#include "../enclave/common_type.h"
#include <toolkit/plugins/csv/csv_reader.h>
typedef ypc::plugins::typed_csv_reader<tax_item_t> tax_reader_t;
impl_csv_reader(tax_reader_t)
