#include "../enclave/common_type.h"
#include <toolkit/plugins/csv/csv_reader.h>
typedef ypc::plugins::typed_csv_reader<industry_item_t> industry_reader_t;
impl_csv_reader(industry_reader_t)
