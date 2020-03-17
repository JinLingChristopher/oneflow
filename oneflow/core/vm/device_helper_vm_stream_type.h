#ifndef ONEFLOW_CORE_VM_DEVICE_HELPER_VM_STREAM_TYPE_H_
#define ONEFLOW_CORE_VM_DEVICE_HELPER_VM_STREAM_TYPE_H_

#include "oneflow/core/vm/vm_stream_type.h"

namespace oneflow {
namespace vm {

class VmScheduler;
class VmInstructionMsg;

class DeviceHelperVmStreamType final : public VmStreamType {
 public:
  DeviceHelperVmStreamType() = default;
  ~DeviceHelperVmStreamType() = default;

  static const VmStreamTypeId kVmStreamTypeId = 3;

  ObjectMsgPtr<VmInstructionMsg> CudaMalloc(uint64_t logical_object_id, size_t size) const;
  ObjectMsgPtr<VmInstructionMsg> CudaFree(uint64_t logical_object_id) const;

  void InitDeviceCtx(std::unique_ptr<DeviceCtx>* device_ctx, VmStream* vm_stream) const override {}

  void InitVmInstructionStatus(const VmStream& vm_stream,
                               VmInstructionStatusBuffer* status_buffer) const override;
  void DeleteVmInstructionStatus(const VmStream& vm_stream,
                                 VmInstructionStatusBuffer* status_buffer) const override;
  bool QueryVmInstructionStatusDone(const VmStream& vm_stream,
                                    const VmInstructionStatusBuffer& status_buffer) const override;
  void Run(VmInstrChain* vm_instr_chain) const override;
};

}  // namespace vm
}  // namespace oneflow

#endif  // ONEFLOW_CORE_VM_DEVICE_HELPER_VM_STREAM_TYPE_H_