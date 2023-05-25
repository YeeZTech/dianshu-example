#include "../enclave/common_type.h"
#include<toolkit/plugins/csv/csv_reader.h>
typedef ypc::plugins::typed_csv_reader<company_label_item_t> company_label_reader_t;
impl_csv_reader(company_label_reader_t)
