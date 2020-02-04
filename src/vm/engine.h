/*
 * engine.h
 * Copyright (C) 4paradigm.com 2019 wangtaize <wangtaize@4paradigm.com>
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *    http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#ifndef SRC_VM_ENGINE_H_
#define SRC_VM_ENGINE_H_

#include <map>
#include <memory>
#include <mutex>  //NOLINT
#include <string>
#include <vector>
#include "base/spin_lock.h"
#include "proto/common.pb.h"
#include "vm/sql_compiler.h"
#include "vm/catalog.h"

namespace fesql {
namespace vm {

using ::fesql::storage::Row;

class Engine;

struct CompileInfo {
    SQLContext sql_ctx;
};


class RunSession {
 public:
    RunSession();

    ~RunSession();

    inline const Schema& GetSchema() const {
        return compile_info_->sql_ctx.schema;
    }

    int32_t Run(std::vector<int8_t*>& buf, uint64_t limit);  // NOLINT

    int32_t RunOne(const Row& in_row, Row& out_row);              // NOLINT

    int32_t RunBatch(std::vector<int8_t*>& buf, uint64_t limit);  // NOLINT

 private:
    inline void SetCompileInfo(const std::shared_ptr<CompileInfo>& compile_info) {
        compile_info_ = compile_info;
    }

    inline void SetCatalog(const std::shared_ptr<Catalog>& cl) { cl_ = cl; }

 private:
    std::shared_ptr<CompileInfo> compile_info_;
    std::shared_ptr<Catalog> cl_;
    friend Engine;
};

typedef std::map<std::string,
                 std::map<std::string, std::shared_ptr<CompileInfo>>>
    EngineCache;
class Engine {
 public:
    explicit Engine(const std::shared_ptr<Catalog>& cl);

    ~Engine();

    bool Get(const std::string& db, const std::string& sql,
             RunSession& session,    // NOLINT
             base::Status& status);  // NOLINT

    std::shared_ptr<CompileInfo> GetCacheLocked(const std::string& db,
                                                const std::string& sql);

 private:
    const std::shared_ptr<Catalog> cl_;
    base::SpinMutex mu_;
    EngineCache cache_;
};

}  // namespace vm
}  // namespace fesql
#endif  // SRC_VM_ENGINE_H_
