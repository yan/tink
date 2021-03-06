// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//
//      http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.
//
////////////////////////////////////////////////////////////////////////////////

#include "tink/integration/awskms/aws_kms_client.h"

#include <cstdlib>
#include <string>
#include <vector>

#include "absl/strings/str_cat.h"
#include "aws/core/Aws.h"
#include "aws/kms/KMSClient.h"
#include "gtest/gtest.h"
#include "tink/util/status.h"
#include "tink/util/statusor.h"

namespace crypto {
namespace tink {
namespace integration {
namespace awskms {
namespace {

using crypto::tink::integration::awskms::AwsKmsClient;

TEST(AwsKmsClientTest, testBasic) {
  std::string aws_key1 = "aws-kms://arn:aws:kms:us-east-1:acc:some/key1";
  std::string aws_key2 = "aws-kms://arn:aws:kms:us-west-2:acc:other/key2";
  std::string non_aws_key = "gcp-kms:://some/gcp/key";
  std::string creds_file = std::string(getenv("TEST_SRCDIR")) +
                      "/tink/testdata/aws_credentials_cc.txt";

  {  // A client not bound to any particular key.
    auto client_result = AwsKmsClient::New("", creds_file);
    EXPECT_TRUE(client_result.ok()) << client_result.status();
    auto client = std::move(client_result.ValueOrDie());
    EXPECT_TRUE(client->DoesSupport(aws_key1));
    EXPECT_TRUE(client->DoesSupport(aws_key2));
    EXPECT_FALSE(client->DoesSupport(non_aws_key));
  }

  {  // A client bound to a specific AWS KMS key.
    auto client_result = AwsKmsClient::New(aws_key1, creds_file);
    EXPECT_TRUE(client_result.ok()) << client_result.status();
    auto client = std::move(client_result.ValueOrDie());
    EXPECT_TRUE(client->DoesSupport(aws_key1));
    EXPECT_FALSE(client->DoesSupport(aws_key2));
    EXPECT_FALSE(client->DoesSupport(non_aws_key));
  }
}


}  // namespace
}  // namespace awskms
}  // namespace integration
}  // namespace tink
}  // namespace crypto
