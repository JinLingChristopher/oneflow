#include "oneflow/core/vm/vm_stream_desc.msg.h"

namespace oneflow {
namespace vm {

void VmStreamDesc::__Init__(VmStreamTypeId vm_stream_type_id, int32_t num_machines,
                            int32_t num_streams_per_machine, int32_t num_streams_per_thread) {
  set_vm_stream_type_id(vm_stream_type_id);
  set_num_machines(num_machines);
  set_num_streams_per_machine(num_streams_per_machine);
  set_num_streams_per_thread(num_streams_per_thread);
}

int32_t VmStreamDesc::num_threads() const {
  int32_t num_devices = num_machines() * num_streams_per_machine();
  CHECK_EQ(num_devices % num_streams_per_thread(), 0);
  return num_devices / num_streams_per_thread();
}

}  // namespace vm
}  // namespace oneflow