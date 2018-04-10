// Copyright 2017 The Crashpad Authors. All rights reserved.
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//
//     http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.

#ifndef CRASHPAD_SNAPSHOT_FUCHSIA_PROCESS_SNAPSHOT_FUCHSIA_H_
#define CRASHPAD_SNAPSHOT_FUCHSIA_PROCESS_SNAPSHOT_FUCHSIA_H_

#include <sys/time.h>
#include <zircon/types.h>

#include <memory>
#include <vector>

#include "base/macros.h"
#include "snapshot/crashpad_info_client_options.h"
#include "snapshot/elf/elf_image_reader.h"
#include "snapshot/elf/module_snapshot_elf.h"
#include "snapshot/fuchsia/process_reader_fuchsia.h"
#include "snapshot/fuchsia/system_snapshot_fuchsia.h"
#include "snapshot/fuchsia/thread_snapshot_fuchsia.h"
#include "snapshot/process_snapshot.h"
#include "snapshot/unloaded_module_snapshot.h"
#include "util/misc/initialization_state_dcheck.h"

namespace crashpad {

//! \brief A ProcessSnapshot of a running (or crashed) process running on a
//!     Fuchsia system. This class is not yet implemented.
class ProcessSnapshotFuchsia : public ProcessSnapshot {
 public:
  ProcessSnapshotFuchsia();
  ~ProcessSnapshotFuchsia() override;

  //! \brief Initializes the object.
  //!
  //! \param[in] process The process handle to create a snapshot from.
  //!
  //! \return `true` if the snapshot could be created, `false` otherwise with
  //!     an appropriate message logged.
  bool Initialize(zx_handle_t process);

  //! \brief Returns options from CrashpadInfo structures found in modules in
  //!     the process.
  //!
  //! \param[out] options Options set in CrashpadInfo structures in modules in
  //!     the process.
  void GetCrashpadOptions(CrashpadInfoClientOptions* options);

  // ProcessSnapshot:
  pid_t ProcessID() const override;
  pid_t ParentProcessID() const override;
  void SnapshotTime(timeval* snapshot_time) const override;
  void ProcessStartTime(timeval* start_time) const override;
  void ProcessCPUTimes(timeval* user_time, timeval* system_time) const override;
  void ReportID(UUID* report_id) const override;
  void ClientID(UUID* client_id) const override;
  const std::map<std::string, std::string>& AnnotationsSimpleMap()
      const override;
  const SystemSnapshot* System() const override;
  std::vector<const ThreadSnapshot*> Threads() const override;
  std::vector<const ModuleSnapshot*> Modules() const override;
  std::vector<UnloadedModuleSnapshot> UnloadedModules() const override;
  const ExceptionSnapshot* Exception() const override;
  std::vector<const MemoryMapRegionSnapshot*> MemoryMap() const override;
  std::vector<HandleSnapshot> Handles() const override;
  std::vector<const MemorySnapshot*> ExtraMemory() const override;

 private:
  // Initializes threads_ on behalf of Initialize().
  void InitializeThreads();

  // Initializes modules_ on behalf of Initialize().
  void InitializeModules();

  internal::SystemSnapshotFuchsia system_;
  std::vector<std::unique_ptr<internal::ThreadSnapshotFuchsia>> threads_;
  std::vector<std::unique_ptr<internal::ModuleSnapshotElf>> modules_;
  ProcessReaderFuchsia process_reader_;
  std::map<std::string, std::string> annotations_simple_map_;
  timeval snapshot_time_;
  InitializationStateDcheck initialized_;

  DISALLOW_COPY_AND_ASSIGN(ProcessSnapshotFuchsia);
};

}  // namespace crashpad

#endif  // CRASHPAD_SNAPSHOT_FUCHSIA_PROCESS_SNAPSHOT_FUCHSIA_H_
