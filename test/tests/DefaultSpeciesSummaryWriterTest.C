
#include "gtest/gtest.h"
#include "prism/prism.h"
#include "fileComparer.h"

class DefaultSpeciesSummaryWriterTest : public testing::Test
{
protected:
  void SetUp() override
  {
    // Save cout's buffer...
    sbuf = std::cout.rdbuf();
    // Redirect cout to our stringstream buffer or any other ostream
    std::cout.rdbuf(buffer.rdbuf());
    prism::NetworkParser::instance().clear();
  }

  void TearDown() override
  {
    // Check if test failed and dump output
    if (HasFailure())
    {
      std::cout.rdbuf(sbuf);
      std::cout << "\n=== CAPTURED OUTPUT ===\n"
                << buffer.str() << "\n=== END CAPTURED OUTPUT ===\n";
    }
    else
    {
      std::cout.rdbuf(sbuf);
    }
    prism::NetworkParser::instance().clear();
  }

  std::stringstream buffer{};
  std::streambuf * sbuf;
};

TEST_F(DefaultSpeciesSummaryWriterTest, DefaultSummary)
{
  auto & np = prism::NetworkParser::instance();

  np.parseNetwork("inputs/simple_argon_rate.yaml");
  np.writeSpeciesSummary("outputs/simple_argon_rate_summary.yaml");
  EXPECT_FILES_EQ("gold/simple_argon_rate_summary.yaml", "outputs/simple_argon_rate_summary.yaml");
}
