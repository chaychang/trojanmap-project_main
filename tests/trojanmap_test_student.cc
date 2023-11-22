#include "gtest/gtest.h"
#include "src/lib/trojanmap.h"

TEST(TrojanMapTest, Autocomplete) {
  TrojanMap m;
  // Test the simple case
  auto names = m.Autocomplete("Chi");
  std::unordered_set<std::string> gt = {"Chick-fil-A", "Chipotle", "Chinese Street Food"}; // groundtruth for "Ch"
  int success = 0;
  for (auto& n: names) {
    EXPECT_EQ(gt.count(n) > 0, true) << n + " is not in gt.";
    if (gt.count(n) > 0){
      success++;
    }
  }
  EXPECT_EQ(success, gt.size());
  // Test the lower case
  names = m.Autocomplete("chi");
  success = 0;
  for (auto& n: names) {
    EXPECT_EQ(gt.count(n) > 0, true) << n + " is not in gt.";
    if (gt.count(n) > 0){
      success++;
    }
  }
  EXPECT_EQ(success, gt.size());
  // Test the lower and upper case 
  names = m.Autocomplete("cHi"); 
  success = 0;
  for (auto& n: names) {
    EXPECT_EQ(gt.count(n) > 0, true) << n + " is not in gt.";
    if (gt.count(n) > 0){
      success++;
    }
  }
  EXPECT_EQ(success, gt.size());
  // Test the upper case 
  names = m.Autocomplete("CHI"); 
  success = 0;
  for (auto& n: names) {
    EXPECT_EQ(gt.count(n) > 0, true) << n + " is not in gt.";
    if (gt.count(n) > 0){
      success++;
    }
  }
  EXPECT_EQ(success, gt.size());
}

// Test FindPosition function
TEST(TrojanMapTest, FindPosition) {
  TrojanMap m;
  
  // Test Chick-fil-A
  auto position = m.GetPosition("Chick-fil-A");
  std::pair<double, double> gt1(34.0167334, -118.2825307); // groundtruth for "Chick-fil-A"
  EXPECT_EQ(position, gt1);
  // Test Ralphs
  position = m.GetPosition("Ralphs");
  std::pair<double, double> gt2(34.0317653, -118.2908339); // groundtruth for "Ralphs"
  EXPECT_EQ(position, gt2);
  // Test Target
  position = m.GetPosition("Target");
  std::pair<double, double> gt3(34.0257016, -118.2843512); // groundtruth for "Target"
  EXPECT_EQ(position, gt3);
  // Test Unknown
  position = m.GetPosition("XXX");
  std::pair<double, double> gt4(-1, -1);
  EXPECT_EQ(position, gt4);
}


TEST(TrojanMapTest, GetAllCategories) {
  TrojanMap m;
  
  auto output = m.GetAllCategories();
  std::set<std::string> expected = {
    "artwork", "attraction", "bakery", "bank", "bar", "beauty", "beverages", "bicycle", "bicycle_rental",
    "bus_station", "cafe", "car", "car_repair", "car_wash", "charging_station", "childcare", "clinic",
    "clothes", "confectionery", "convenience", "copyshop", "dentist", "department_store", "driving_school",
    "fabric", "fast_food", "food_court", "fountain", "fuel", "gallery", "hairdresser", "hospital", "hotel",
    "library", "marketplace", "mobile_phone", "museum", "music", "optician", "parcel_locker", "parking",
    "parking_entrance", "pharmacy", "place_of_worship", "police", "post_box", "post_office", "restaurant",
    "school", "shoe_repair", "shoes", "skate", "social_facility", "supermarket", "theatre", "tobacco",
    "yoga", "yes"
  };
  std::set<std::string> output_set(output.begin(), output.end());
  EXPECT_EQ(output_set, expected);
}
TEST(TrojanMapTest, GetAllLocationsFromCategory) {
  TrojanMap m;
  
  auto output = m.GetAllLocationsFromCategory("bank");
  std::set<std::string> expected = {"9591449441", "5237417651", "9591449465"};
  std::set<std::string> output_set(output.begin(), output.end());
  EXPECT_EQ(output_set, expected);
}

TEST(TrojanMapTest, GetAllLocationsFromCategory2) {
  TrojanMap m;
  
  auto output = m.GetAllLocationsFromCategory("supermarket");
  std::set<std::string> expected = {"5237417649", "2578244375", "7158034317", "6045067406"};
  std::set<std::string> output_set(output.begin(), output.end());
  EXPECT_EQ(output_set, expected);
}


TEST(TrojanMapTest, GetLocationRegex1) {
  TrojanMap m;
  std::set<std::string> expected_set;
  auto actual = m.GetLocationRegex(std::regex("dummy"));
  std::set<std::string> actual_set(actual.begin(), actual.end());
  EXPECT_EQ(actual_set, expected_set);
}

TEST(TrojanMapTest, GetLocationRegex2) {
  TrojanMap m;
  std::set<std::string> expected_set = { "2578244375" };
  auto actual = m.GetLocationRegex(std::regex("Ra.?ph.+"));
  std::set<std::string> actual_set(actual.begin(), actual.end());
  EXPECT_EQ(expected_set, actual_set);
}

TEST(TrojanMapTest, GetLocationRegex3) {
  TrojanMap m;
  std::set<std::string> expected_set = { "9596557990", "9596557987", "9462770200" };
  auto actual = m.GetLocationRegex(std::regex(".?da.{1,10}"));
  std::set<std::string> actual_set(actual.begin(), actual.end());
  EXPECT_EQ(expected_set, actual_set);
}

// Test cycle detection function
TEST(TrojanMapTest, TopologicalSort) {
  TrojanMap m;
  
  std::vector<std::string> location_names = {"Ralphs", "Chick-fil-A", "KFC"};
  std::vector<std::vector<std::string>> dependencies = {{"Ralphs","KFC"}, {"Ralphs","Chick-fil-A"}, {"KFC","Chick-fil-A"}};
  auto result = m.DeliveringTrojan(location_names, dependencies);
  std::vector<std::string> gt ={"Ralphs", "KFC","Chick-fil-A"};
  EXPECT_EQ(result, gt);
}

TEST(TrojanMapTest, TopologicalSort2) {
  TrojanMap m;
  
  std::vector<std::string> location_names = {"0", "1", "2", "3"};
  std::vector<std::vector<std::string>> dependencies = {{"0","1"}, {"1","2"}, {"2","3"}};
  auto result = m.DeliveringTrojan(location_names, dependencies);
  std::vector<std::string> gt ={"0", "1", "2", "3"};
  EXPECT_EQ(result, gt);
}

TEST(TrojanMapTest, TopologicalSort3) {
  TrojanMap m;
  
  std::vector<std::string> location_names = {"0", "1", "2", "3", "4"};
  std::vector<std::vector<std::string>> dependencies = {{"0","1"}, {"0","2"}, {"1","2"}, {"1","3"}, {"2","3"}, {"2","4"}};
  auto result = m.DeliveringTrojan(location_names, dependencies);
  std::vector<std::string> gt ={"0", "1", "2", "3", "4"};
  EXPECT_EQ(result, gt);
}

TEST(TrojanMapTest, TopologicalSort4) {
  TrojanMap m;
  
  std::vector<std::string> location_names = {"0", "1", "2", "3", "4"};
  std::vector<std::vector<std::string>> dependencies = {{"0","1"}, {"0","2"}, {"1","2"}, {"1","3"}, {"2","3"}, {"2","0"}};
  auto result = m.DeliveringTrojan(location_names, dependencies);
  std::vector<std::string> gt ={};
  EXPECT_EQ(result, gt);
}

TEST(TrojanMapTest, TopologicalSort5) {
  TrojanMap m;
  
  std::vector<std::string> location_names = {"1", "2", "3", "4", "0"};
  std::vector<std::vector<std::string>> dependencies = {{"0","1"}, {"0","2"}, {"1","2"}, {"1","3"}, {"2","3"}, {"2","4"}};
  auto result = m.DeliveringTrojan(location_names, dependencies);
  std::vector<std::string> gt ={"0", "1", "2", "3", "4"};
  EXPECT_EQ(result, gt);
}

// Test CalculateShortestPath_Dijkstra function
TEST(TrojanMapTest, CalculateShortestPath_Dijkstra) {
  TrojanMap m;
  
  // Test from Ralphs to Chick-fil-A
  auto path = m.CalculateShortestPath_Dijkstra("Ralphs", "Chick-fil-A");
  std::vector<std::string> gt{
      "2578244375","4380040154","4380040153","4380040152","4380040148","6818427920","6818427919",
      "6818427918","6818427892","6818427898","6818427917","6818427916","7232024780","6813416145",
      "6813416154","6813416153","6813416152","6813416151","6813416155","6808069740","6816193785",
      "6816193786","123152294","4015203136","4015203134","4015203133","21098539","6389467809",
      "4015203132","3195897587","4015203129","4015203127","6352865690","6813379589","6813379483",
      "3402887081","6814958394","3402887080","602606656","4872897515","4399697589","6814958391",
      "123209598","6787673296","122728406","6807762271","4399697304","4399697302","5231967015",
      "1862347583","3233702827","4540763379","6819179753","6820935900","6820935901","6813379556",
      "6820935898","1781230450","1781230449","4015405542","4015405543","1837212104","1837212107",
      "2753199985","6820935907","1837212100","4015372458","6813411588","1837212101","6814916516",
      "6814916515","6820935910","4547476733"}; // Expected path
  // Print the path lengths
  std::cout << "My path length: "  << m.CalculatePathLength(path) << "miles" << std::endl;
  std::cout << "GT path length: " << m.CalculatePathLength(gt) << "miles" << std::endl;
  EXPECT_EQ(path, gt);
  
  // Reverse the input from Ralphs to Chick-fil-A
  path = m.CalculateShortestPath_Dijkstra("Chick-fil-A", "Ralphs");
  std::reverse(gt.begin(),gt.end()); // Reverse the path

  // Print the path lengths
  std::cout << "My path length: "  << m.CalculatePathLength(path) << "miles" << std::endl;
  std::cout << "GT path length: " << m.CalculatePathLength(gt) << "miles" << std::endl;
  EXPECT_EQ(path, gt);
}

// Test CalculateEditDistance function
TEST(TrojanMapTest, CalculateEditDistance) {
  TrojanMap m;
  EXPECT_EQ(m.CalculateEditDistance("horse", "ros"), 3);
  EXPECT_EQ(m.CalculateEditDistance("intention", "execution"), 5);
}

// Test FindClosestName function
TEST(TrojanMapTest, FindClosestName) {
  TrojanMap m;
  EXPECT_EQ(m.FindClosestName("Rolphs"), "Ralphs");
  EXPECT_EQ(m.FindClosestName("Targeety"), "Target");
}

// Test CalculateShortestPath_Bellman_Ford function
TEST(TrojanMapTest, CalculateShortestPath_Bellman_Ford) {
  TrojanMap m;
  
  // Test from Ralphs to Chick-fil-A
  auto path = m.CalculateShortestPath_Bellman_Ford("Ralphs", "Chick-fil-A");
  std::vector<std::string> gt{
      "2578244375","4380040154","4380040153","4380040152","4380040148","6818427920","6818427919",
      "6818427918","6818427892","6818427898","6818427917","6818427916","7232024780","6813416145",
      "6813416154","6813416153","6813416152","6813416151","6813416155","6808069740","6816193785",
      "6816193786","123152294","4015203136","4015203134","4015203133","21098539","6389467809",
      "4015203132","3195897587","4015203129","4015203127","6352865690","6813379589","6813379483",
      "3402887081","6814958394","3402887080","602606656","4872897515","4399697589","6814958391",
      "123209598","6787673296","122728406","6807762271","4399697304","4399697302","5231967015",
      "1862347583","3233702827","4540763379","6819179753","6820935900","6820935901","6813379556",
      "6820935898","1781230450","1781230449","4015405542","4015405543","1837212104","1837212107",
      "2753199985","6820935907","1837212100","4015372458","6813411588","1837212101","6814916516",
      "6814916515","6820935910","4547476733"}; // Expected path
  // Print the path lengths
  std::cout << "My path length: "  << m.CalculatePathLength(path) << "miles" << std::endl;
  std::cout << "GT path length: " << m.CalculatePathLength(gt) << "miles" << std::endl;
  EXPECT_EQ(path, gt);
  
  // Reverse the input from Ralphs to Chick-fil-A
  path = m.CalculateShortestPath_Bellman_Ford("Chick-fil-A", "Ralphs");
  std::reverse(gt.begin(),gt.end()); // Reverse the path

  // Print the path lengths
  std::cout << "My path length: "  << m.CalculatePathLength(path) << "miles" << std::endl;
  std::cout << "GT path length: " << m.CalculatePathLength(gt) << "miles" << std::endl;
  EXPECT_EQ(path, gt);
}