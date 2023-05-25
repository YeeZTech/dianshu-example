#include "../enclave/common_type.h"
#include<toolkit/plugins/csv/csv_reader.h>
typedef ypc::plugins::typed_csv_reader<county_item_t> county_reader_t;
impl_csv_reader(county_reader_t)
