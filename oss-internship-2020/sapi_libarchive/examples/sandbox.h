#ifndef SAPI_LIBARCHIVE_SANDBOX_H
#define SAPI_LIBARCHIVE_SANDBOX_H

#include <syscall.h>

#include "libarchive_sapi.sapi.h"

class SapiLibarchiveSandboxCreate : public LibarchiveSandbox {
 public:
  // TODO
  explicit SapiLibarchiveSandboxCreate() {}

 private:
  std::unique_ptr<sandbox2::Policy> ModifyPolicy(
      sandbox2::PolicyBuilder*) override {
    return sandbox2::PolicyBuilder().BuildOrDie();
  }
};

class SapiLibarchiveSandboxExtract : public LibarchiveSandbox {
 public:
  // TODO
  explicit SapiLibarchiveSandboxExtract(const std::string &archive_path, const int do_extract) 
    : archive_path_(archive_path), do_extract_(do_extract) {}

 private:
  virtual void ModifyExecutor(sandbox2::Executor* executor) override {
    // TODO create /output/ + chdir here if do_execute
    if (do_extract_) {
        // TODO change the directory
        std::cout << "inside executor do extract" << std::endl;
    } else {
        // Do nothing since we do not need to create any files
    }
  }

  std::unique_ptr<sandbox2::Policy> ModifyPolicy(
      sandbox2::PolicyBuilder*) override {
        auto policy = sandbox2::PolicyBuilder()
        .AllowRead()
        .AllowWrite()
        .AllowOpen()
        .AllowSystemMalloc()
        .AllowGetIDs()
        .AllowSafeFcntl()
        .AllowStat()
        .AllowExit()
        .AllowSyscalls({
            __NR_futex,
            __NR_lseek,
            __NR_close,
            __NR_gettid,
            __NR_umask,
        })
        .AddFile(archive_path_);

        if (do_extract_) {
            // map "/output/" to cwd
            std::cout << "do extract inside policy" << std::endl;
        }
    return policy.BuildOrDie();
  }

  const std::string archive_path_;
  const int do_extract_;
};

#endif  // SAPI_LIBARCHIVE_SANDBOX_H