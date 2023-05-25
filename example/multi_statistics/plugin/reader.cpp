#include "../enclave/common_type.h"
#include<toolkit/plugins/csv/csv_reader.h>
typedef ypc::plugins::typed_csv_reader<value_item_t> multi_statistics_reader_t;
impl_csv_reader(multi_statistics_reader_t)
