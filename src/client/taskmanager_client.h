/*
 * Copyright 2021 4Paradigm
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *   http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#ifndef SRC_CLIENT_TASKMANAGER_CLIENT_H_
#define SRC_CLIENT_TASKMANAGER_CLIENT_H_

#include <string>
#include "base/status.h"
#include "client/client.h"
#include "proto/taskmanager.pb.h"
#include "rpc/rpc_client.h"

namespace openmldb {
namespace client {

class TaskManagerClient : public Client {
 public:
    TaskManagerClient(const std::string& endpoint, const std::string& real_endpoint, bool use_sleep_policy)
        : Client(endpoint, real_endpoint),
        client_(real_endpoint.empty() ? endpoint : real_endpoint, use_sleep_policy) {}

    ~TaskManagerClient() {}

    int Init() override {
        return client_.Init();
    }

    ::openmldb::base::Status ShowJobs(const bool onlyUnfinished,
                                      std::vector<::openmldb::taskmanager::JobInfo>& jobInfos);

 private:
    ::openmldb::RpcClient<::openmldb::taskmanager::TaskManagerServer_Stub> client_;
};

}  // namespace client
}  // namespace openmldb

#endif  // SRC_CLIENT_TASKMANAGER_CLIENT_H_
