#include "../enclave/car_type.h"
#include <toolkit/plugins/csv/csv_reader.h>
typedef ypc::plugins::typed_csv_reader<sjhd_log_item_t> sjhd_log_reader_t;
impl_csv_reader(sjhd_log_reader_t)
