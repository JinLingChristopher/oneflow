#include "oneflow/core/kernel/decode_random_kernel.h"

namespace oneflow {

namespace {

void RandomFillBlob(DeviceCtx* ctx, DeviceType device_type, const InitializerConf& initializer_conf,
                    uint32_t random_seed, Blob* blob) {
  static const HashMap<std::string,
                       void (*)(DeviceCtx * ctx, const InitializerConf& initializer_conf,
                                uint32_t random_seed, Blob* blob)>
      fill_funcs = {
#define RANDOM_FILL_ENTRY(type_dev, data_type_pair)         \
  {GetHashKey(type_dev, OF_PP_PAIR_SECOND(data_type_pair)), \
   &KernelUtil<type_dev, OF_PP_PAIR_FIRST(data_type_pair)>::InitializeWithConf},
          OF_PP_SEQ_PRODUCT_FOR_EACH_TUPLE(RANDOM_FILL_ENTRY, DEVICE_TYPE_SEQ,
                                           ARITHMETIC_DATA_TYPE_SEQ)};
  fill_funcs.at(GetHashKey(device_type, blob->data_type()))(ctx, initializer_conf, random_seed,
                                                            blob);
}

}  // namespace

template<DeviceType device_type>
void DecodeRandomKernel<device_type>::VirtualKernelInit(const ParallelContext*) {
  gen_.reset(new std::mt19937(this->kernel_conf().decode_random_conf().random_seed()));
  dis_.reset(new std::uniform_int_distribution<uint32_t>());
  is_init_ = false;
}

template<DeviceType device_type>
uint32_t DecodeRandomKernel<device_type>::GenNextRandomSeed() const {
  return (*dis_)(*gen_);
}

template<DeviceType device_type>
void DecodeRandomKernel<device_type>::Forward(
    const KernelCtx& ctx, std::function<Blob*(const std::string&)> BnInOp2Blob) const {
  const DecodeRandomOpConf& conf = this->op_conf().decode_random_conf();
  if (is_init_ == false) {
    RandomFillBlob(ctx.device_ctx, device_type, conf.data_initializer(), this->GenNextRandomSeed(),
                   BnInOp2Blob("out"));
    is_init_ = true;
  }
}

REGISTER_KERNEL_WITH_DEVICE(OperatorConf::kDecodeRandomConf, DeviceType::kCPU,
                            DecodeRandomKernel<DeviceType::kCPU>);
REGISTER_KERNEL_WITH_DEVICE(OperatorConf::kDecodeRandomConf, DeviceType::kGPU,
                            DecodeRandomKernel<DeviceType::kGPU>);

}  // namespace oneflow
