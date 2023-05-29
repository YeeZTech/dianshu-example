#include "../enclave/common_type.h"
#include <toolkit/plugins/csv/csv_reader.h>
typedef ypc::plugins::typed_csv_reader<dic_ent_type_item_t>
    dic_ent_type_reader_t;
impl_csv_reader(dic_ent_type_reader_t)
