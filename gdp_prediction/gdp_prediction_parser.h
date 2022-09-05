
#include "ypc/stbox/ebyte.h"
#include "ypc/stbox/stx_common.h"
#include "ypc/core_t/analyzer/data_source.h"
#include "ypc/stbox/tsgx/log.h"
#include "user_type.h"

#include "ypc/corecommon/to_type.h"
#include <hpda/extractor/raw_data.h>
#include <hpda/output/memory_output.h>
#include <hpda/processor/query/filter.h>

typedef ff::net::ntpackage<0, radiance_sum> nt_package_t;

class gdp_prediction_parser {
public:
  inline stbox::bytes do_parse(const stbox::bytes &param) {
    auto pkg = ypc::make_package<nt_package_t>::from_bytes(param);
    double slope = 4787.502598784844;
    double vertical_intercept = -80.42706901157614;
    auto rad = pkg.get<radiance_sum>();
    auto radiance = std::stod( rad );
    auto res = slope * radiance + vertical_intercept;
    auto temp = std::to_string( res );
    stbox::bytes result( temp );
    LOG(INFO) << "do parse done";
    return result;
  }
};

