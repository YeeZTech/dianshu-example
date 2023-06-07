#include "../enclave/car_type.h"
#include <toolkit/plugins/csv/csv_reader.h>
typedef ypc::plugins::typed_csv_reader<sjdx_log_item_t> sjdx_log_reader_t;
impl_csv_reader(sjdx_log_reader_t)
