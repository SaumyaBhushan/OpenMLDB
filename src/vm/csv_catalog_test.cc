/*
 * csv_catalog_test.cc
 * Copyright (C) 4paradigm.com 2020 wangtaize <wangtaize@4paradigm.com>
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

#include "vm/csv_catalog.h"
#include "gtest/gtest.h"
#include "arrow/filesystem/localfs.h"

namespace fesql {
namespace vm {
class CSVCatalogTest : public ::testing::Test {

 public:
    CSVCatalogTest() {}
    ~CSVCatalogTest() {}
};


TEST_F(CSVCatalogTest, test_handler_init) { 
    std::string table_dir = "./table1";
    std::string table_name = "table1";
    std::string db = "db1";
    std::shared_ptr<::arrow::fs::FileSystem> fs(new arrow::fs::LocalFileSystem());
    CSVTableHandler handler(table_dir, table_name, db, fs);
    bool ok = handler.Init();
    ASSERT_TRUE(ok);
    storage::RowView rv(handler.GetSchema());
    auto it = handler.GetIterator();
    while (it->Valid()) {
        auto value = it->GetValue();
        rv.Reset(reinterpret_cast<const int8_t*>(value.data()), value.size());
        char* data = NULL;
        uint32_t size = 0;
        rv.GetString(0, &data, &size);
        std::string view(data, size);
        std::cout<< view << std::endl;
        rv.GetString(1, &data, &size);
        std::string view2(data, size);
        std::cout<< view2 << std::endl;
        it->Next();
    }
}

} // namespace vm
} // namepsace fesql

int main(int argc, char **argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
