#ifndef ONEFLOW_CORE_VM_VM_STREAM_TYPE_H_
#define ONEFLOW_CORE_VM_VM_STREAM_TYPE_H_

#include <string>
#include "oneflow/core/vm/vm_stream_desc.msg.h"
#include "oneflow/core/vm/vm_instruction_id.msg.h"
#include "oneflow/core/common/callback.msg.h"
#include "oneflow/core/device/device_context.h"

namespace oneflow {

class ObjectMsgAllocator;

namespace vm {

class VmStream;
class VmInstructionStatusBuffer;
class VmInstrChain;

class VmStreamType {
 public:
  virtual ~VmStreamType() = default;

  virtual void InitDeviceCtx(std::unique_ptr<DeviceCtx>* device_ctx, VmStream* vm_stream) const = 0;

  virtual void InitVmInstructionStatus(const VmStream& vm_stream,
                                       VmInstructionStatusBuffer* status_buffer) const = 0;
  virtual void DeleteVmInstructionStatus(const VmStream& vm_stream,
                                         VmInstructionStatusBuffer* status_buffer) const = 0;
  virtual bool QueryVmInstructionStatusDone(
      const VmStream& vm_stream, const VmInstructionStatusBuffer& status_buffer) const = 0;
  virtual void Run(VmInstrChain* vm_instr_chain) const = 0;

 protected:
  VmStreamType() = default;
};

const VmStreamType* LookupVmStreamType(VmStreamTypeId);
void RegisterVmStreamType(VmStreamTypeId, const VmStreamType*);
template<typename T>
void RegisterVmStreamType() {
  RegisterVmStreamType(T::kVmStreamTypeId, new T());
}

class VmInstructionId;

const VmInstructionId& LookupVmInstructionId(const std::string& instr_type_name);
void RegisterVmInstructionId(const std::string& instr_type_name, VmStreamTypeId vm_stream_type_id,
                             VmInstructionOpcode opcode, VmType vm_type);
template<typename T>
void RegisterVmInstructionId(const std::string& instr_type_name, VmInstructionOpcode opcode,
                             VmType vm_type) {
  RegisterVmInstructionId(instr_type_name, T::kVmStreamTypeId, opcode, vm_type);
}

}  // namespace vm
}  // namespace oneflow

#endif  // ONEFLOW_CORE_VM_VM_STREAM_TYPE_H_