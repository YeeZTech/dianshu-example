#include "../enclave/common_type.h"
#include <toolkit/plugins/csv/csv_reader.h>
typedef ypc::plugins::typed_csv_reader<dic_industry_item_t>
    dic_industry_reader_t;
impl_csv_reader(dic_industry_reader_t)
