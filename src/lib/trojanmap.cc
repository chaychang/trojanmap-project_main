#include "trojanmap.h"

//-----------------------------------------------------
// TODO: Students should implement the following:
//-----------------------------------------------------
/**
 * GetLat: Get the latitude of a Node given its id. If id does not exist, return
 * -1.
 *
 * @param  {std::string} id : location id
 * @return {double}         : latitude
 */
double TrojanMap::GetLat(const std::string &id) { 
  for(auto it = data.begin(); it!= data.end(); it++){
    if(it->first == id){
      return it->second.lat;
    }
  }
  return -1;
}

/**
 * GetLon: Get the longitude of a Node given its id. If id does not exist,
 * return -1.
 *
 * @param  {std::string} id : location id
 * @return {double}         : longitude
 */
double TrojanMap::GetLon(const std::string &id) {
  for(auto it = data.begin(); it!= data.end(); it++){
    if(it->first == id){
      return it->second.lon;
    }
  }
  return -1;
}

/**
 * GetName: Get the name of a Node given its id. If id does not exist, return
 * "NULL".
 *
 * @param  {std::string} id : location id
 * @return {std::string}    : name
 */
std::string TrojanMap::GetName(const std::string &id) {
  for(auto it = data.begin(); it!= data.end(); it++){
    if(it->first == id){
      return it->second.name;
    }
  }
  return "NULL";
}

/**
 * GetNeighborIDs: Get the neighbor ids of a Node. If id does not exist, return
 * an empty vector.
 *
 * @param  {std::string} id            : location id
 * @return {std::vector<std::string>}  : neighbor ids
 */
std::vector<std::string> TrojanMap::GetNeighborIDs(const std::string &id) {
  std::vector<std::string> neighbour_ids;
  for(auto it = data.begin(); it!= data.end(); it++){
    if(it->first == id){
      // for(auto i: it->second.neighbors){
      //   neighbour_ids.push_back(i);
      // }
      return it->second.neighbors;
    }
  }
  return {};
}

/**
 * GetID: Given a location name, return the id.
 * If the node does not exist, return an empty string.
 * The location name must be unique, which means there is only one node with the name.
 *
 * @param  {std::string} name          : location name
 * @return {std::string}               : id
 */
std::string TrojanMap::GetID(const std::string &name) {
  std::string res = "";
  for(auto it = data.begin(); it != data.end(); it++){
    if(it->second.name == name){
      res = it->first;
    }
  }
  return res;
}

/**
 * GetPosition: Given a location name, return the position. If id does not
 * exist, return (-1, -1).
 *
 * @param  {std::string} name          : location name
 * @return {std::pair<double,double>}  : (lat, lon)
 */
std::pair<double, double> TrojanMap::GetPosition(std::string name) {
  std::pair<double, double> results(-1, -1);
  for(auto it = data.begin(); it != data.end(); it++){
    if(it->second.name == name){
      results.first = it->second.lat;
      results.second = it->second.lon;
    }
  }
  return results;
}

/**
 * CalculateEditDistance: Calculate edit distance between two location names
 * @param  {std::string} a          : first string
 * @param  {std::string} b          : second string
 * @return {int}                    : edit distance between two strings
 */
int TrojanMap::CalculateEditDistance(std::string a, std::string b) {  
  int size1 = a.size();
  int size2 = b.size();
  std::vector<std::vector<int>> dp(size1 + 1, std::vector<int>(size2 + 1));
  for(int i = 0; i <= size1; i++){
    dp[i][0] = i;
  }
  for(int j = 0; j <= size2; j++){
    dp[0][j] = j;
  }
  for (int i = 1; i <= size1; i++){
    for (int j = 1; j <= size2; j++){
      if (a[i - 1] == b[j - 1]){
        dp[i][j] = dp[i - 1][j - 1];
      }else{
        dp[i][j] = 1 + std::min({dp[i - 1][j], dp[i][j - 1], dp[i - 1][j - 1]});
      }
    }
  }
  return dp[size1][size2];
}

/**
 * FindClosestName: Given a location name, return the name with the smallest edit
 * distance.
 *
 * @param  {std::string} name          : location name
 * @return {std::string} tmp           : the closest name
 */
std::string TrojanMap::FindClosestName(std::string name) {
  std::string tmp = ""; // Start with a dummy word
  int min_dis = CalculateEditDistance(name, tmp);
  for (auto it = data.begin(); it != data.end(); it++){
    int dis = CalculateEditDistance(it->second.name, name);
    if (dis < min_dis){
      min_dis = dis;
      tmp = it->second.name;
    }
  }
  return tmp;
}

/**
 * Autocomplete: Given a parital name return all the possible locations with
 * partial name as the prefix. The function should be case-insensitive.
 *
 * @param  {std::string} name          : partial name
 * @return {std::vector<std::string>}  : a vector of full names
 */
std::vector<std::string> TrojanMap::Autocomplete(std::string name) {
  std::vector<std::string> results;
  for(auto &elem : name){
    elem = std::tolower(elem);
  }
  for (auto it = data.begin(); it != data.end(); it++){
    if (it->second.name.empty()){
      continue;
    }else{
      std::string new_data = it->second.name;
      for (auto &elem : new_data){
        elem = std::tolower(elem);
      }
      if (name == new_data.substr(0, name.size())){
        results.push_back(it->second.name);
      }
    }
  }
  return results;
}

/**
 * GetAllCategories: Return all the possible unique location categories, i.e.
 * there should be no duplicates in the output.
 *
 * @return {std::vector<std::string>}  : all unique location categories
 */
std::vector<std::string> TrojanMap::GetAllCategories() {
  std::set<std::string> unique_categories;
  std::vector<std::string> return_unique_categories;
  for(auto it = data.begin(); it != data.end(); it++){
    for(auto i: it->second.attributes){
      unique_categories.insert(i);
    }
  }
  for(auto i: unique_categories){
    return_unique_categories.push_back(i);
  }
  return return_unique_categories;
}

/**
 * GetAllLocationsFromCategory: Return all the locations of the input category (i.e.
 * 'attributes' in data.csv). If there is no location of that category, return
 * (-1, -1). The function should be case-insensitive.
 *
 * @param  {std::string} category         : category name (attribute)
 * @return {std::vector<std::string>}     : ids
 */
std::vector<std::string> TrojanMap::GetAllLocationsFromCategory(
    std::string category) {
  std::vector<std::string> res;
  for (auto it = data.begin(); it != data.end(); it++){
    for(auto elem : it->second.attributes){
      if (elem == category){
        res.push_back(it->second.id);
      }
    }
  }
  return res;
}

/**
 * GetLocationRegex: Given the regular expression of a location's name, your
 * program should first check whether the regular expression is valid, and if so
 * it returns all locations that match that regular expression.
 *
 * @param  {std::regex} location name      : the regular expression of location
 * names
 * @return {std::vector<std::string>}     : ids
 */
std::vector<std::string> TrojanMap::GetLocationRegex(std::regex location) {
  std::vector<std::string> regex_matching_location;
  for(auto it = data.begin(); it != data.end(); it++){
    if(regex_match(it->second.name,location)){
      regex_matching_location.push_back(it->second.id);
    }
  }
  return regex_matching_location;
}

/**
 * CalculateDistance: Get the distance between 2 nodes.
 * We have provided the code for you. Please do not need to change this function.
 * You can use this function to calculate the distance between 2 nodes.
 * The distance is in mile.
 * The distance is calculated using the Haversine formula.
 * https://en.wikipedia.org/wiki/Haversine_formula
 * 
 * @param  {std::string} a  : a_id
 * @param  {std::string} b  : b_id
 * @return {double}  : distance in mile
 */
double TrojanMap::CalculateDistance(const std::string &a_id,
                                    const std::string &b_id) {
  // Do not change this function
  Node a = data[a_id];
  Node b = data[b_id];
  double dlon = (b.lon - a.lon) * M_PI / 180.0;
  double dlat = (b.lat - a.lat) * M_PI / 180.0;
  double p = pow(sin(dlat / 2), 2.0) + cos(a.lat * M_PI / 180.0) *
                                           cos(b.lat * M_PI / 180.0) *
                                           pow(sin(dlon / 2), 2.0);
  double c = 2 * asin(std::min(1.0, sqrt(p)));
  return c * 3961;
}

/**
 * CalculatePathLength: Calculates the total path length for the locations
 * inside the vector.
 * We have provided the code for you. Please do not need to change this function.
 * 
 * @param  {std::vector<std::string>} path : path
 * @return {double}                        : path length
 */
double TrojanMap::CalculatePathLength(const std::vector<std::string> &path) {
  // Do not change this function
  double sum = 0;
  for (int i = 0; i < int(path.size()) - 1; i++) {
    sum += CalculateDistance(path[i], path[i + 1]);
  }
  return sum;
}

/**
 * CalculateShortestPath_Dijkstra: Given 2 locations, return the shortest path
 * which is a list of id. Hint: Use priority queue.
 *
 * @param  {std::string} location1_name     : start
 * @param  {std::string} location2_name     : goal
 * @return {std::vector<std::string>}       : path
 */
std::vector<std::string> TrojanMap::CalculateShortestPath_Dijkstra(
    std::string location1_name, std::string location2_name) {
  std::vector<std::string> path;
  std::unordered_map<std::string, double> path_and_cost;
  std::unordered_map<std::string, std::string> prev_nodes;
  std::priority_queue<std::pair<double, std::string>, std::vector<std::pair<double, std::string>>, std::greater<std::pair<double, std::string>>> pq;

  for(auto i: data){
    if(i.first == GetID(location1_name)){
      path_and_cost[i.first] = 0;
    }
    else{
      path_and_cost[i.first] = INT_MAX;
    }
  }
  
  pq.push(std::make_pair(0, GetID(location1_name))); //adding starting node

  while(pq.size()!=0){
    auto top_element = pq.top(); //get elements of top of priority queue
    pq.pop(); //pop from top

    double current_distance = top_element.first;

    std::vector<std::string> neighbour_ids = GetNeighborIDs(top_element.second); // get its neighbours 

    for(auto i: neighbour_ids){
      double euclidean_distance = CalculateDistance(top_element.second, i);
      double distance_to_neighbour = current_distance + euclidean_distance;

      if (distance_to_neighbour < path_and_cost[i]) {
          path_and_cost[i] = distance_to_neighbour;
          pq.push(std::make_pair(distance_to_neighbour, i));
          prev_nodes[i] = top_element.second;
      }
    }
  }

  std::string current_node = GetID(location2_name);
  while(current_node!=GetID(location1_name)){
    path.push_back(current_node);
    if(prev_nodes.find(current_node) != prev_nodes.end()){
      current_node = prev_nodes[current_node];
    }
    else {
      path.clear();
      break;
    }
  }

  path.push_back(GetID(location1_name));
  std::reverse(path.begin(), path.end());

  return path;
}

/**
 * CalculateShortestPath_Bellman_Ford: Given 2 locations, return the shortest
 * path which is a list of id. Hint: Do the early termination when there is no
 * change on distance.
 *
 * @param  {std::string} location1_name     : start
 * @param  {std::string} location2_name     : goal
 * @return {std::vector<std::string>}       : path
 */
std::vector<std::string> TrojanMap::CalculateShortestPath_Bellman_Ford(
    std::string location1_name, std::string location2_name) {
  std::vector<std::string> path;
  std::unordered_map<std::string, double> distances;
  std::unordered_map<std::string, std::string> before_node;
  std::string location1_id = GetID(location1_name);
  std::string location2_id = GetID(location2_name);
  if(location1_id.empty() || location2_id.empty()) {
    return {};
  }

  for(auto node : data){
    distances[node.first] = INT_MAX;
    before_node[node.first] = "";
  }
  distances[location1_id] = 0.0;

  for(int i = 0; i < data.size() - 1; i++){
    bool update = false;
    for(auto node : data){
      if(distances[node.first] == INT_MAX){
        continue;
      }
      for(auto neighbor_id : node.second.neighbors){
        double distance = CalculateDistance(node.first, neighbor_id);
        if (distances[node.first] + distance < distances[neighbor_id]){
          distances[neighbor_id] = distances[node.first] + distance;
          before_node[neighbor_id] = node.first;
          update = true;
        }
      }
    }
    if(!update){
      break;
    }
  }
  for(auto current = location2_id; current != location1_id; current = before_node[current]) {
    if(before_node[current] == ""){
      return {};
    }
    path.push_back(current);
  }
  path.push_back(location1_id);
  std::reverse(path.begin(), path.end());
  return path;
}

/**
 * Traveling salesman problem: Given a list of locations, return the shortest
 * path which visit all the places and back to the start point.
 *
 * @param  {std::vector<std::string>} input : a list of locations needs to visit
 * @return {std::pair<double, std::vector<std::vector<std::string>>} : a pair of total distance and the all the progress to get final path, 
 *                                                                      for example: {10.3, {{0, 1, 2, 3, 4, 0}, {0, 1, 2, 3, 4, 0}, {0, 4, 3, 2, 1, 0}}},
 *                                                                      where 10.3 is the total distance, 
 *                                                                      and the first vector is the path from 0 and travse all the nodes and back to 0,
 *                                                                      and the second vector is the path shorter than the first one,
 *                                                                      and the last vector is the shortest path.
 */
// Please use brute force to implement this function, ie. find all the permutations.
std::pair<double, std::vector<std::vector<std::string>>> TrojanMap::TravelingTrojan_Brute_force(
                                    std::vector<std::string> location_ids) {
  std::pair<double, std::vector<std::vector<std::string>>> records;
  double shortest_dist = INT_MAX;
  std::vector<std::vector<std::string>> path_perm_storage;
  std::vector<std::string> current_path;
  std::string location_id_start = location_ids[0];

  location_ids.erase(location_ids.begin());
  std::sort(location_ids.begin(), location_ids.end());

  do {  
      std::vector<std::string> current_path;
      double total_distance = 0;
      total_distance += CalculateDistance(location_id_start, location_ids[0]);
      current_path.push_back(location_id_start);

      for(int k=0; k<location_ids.size()-1; k++){
          total_distance += CalculateDistance(location_ids[k], location_ids[k+1]);
          current_path.push_back(location_ids[k]);
      }

      total_distance += CalculateDistance(location_ids[location_ids.size()-1], location_id_start);
      if(total_distance<shortest_dist) shortest_dist = total_distance;
      current_path.push_back(location_ids[location_ids.size()-1]);
      current_path.push_back(location_id_start);
      path_perm_storage.push_back(current_path);

  } while(next_permutation(location_ids.begin(), location_ids.end()));

  std::sort(path_perm_storage.begin(), path_perm_storage.end(), [this](std::vector<std::string> &path1, std::vector<std::string> &path2) {
        return CalculatePathLength(path1) > CalculatePathLength(path2);
  });
  
  std::cout<<shortest_dist<<std::endl;
  records.first = shortest_dist;
  records.second = path_perm_storage;
  return records;
}

// Please use backtracking to implement this function
std::pair<double, std::vector<std::vector<std::string>>> TrojanMap::TravelingTrojan_Backtracking(
                                    std::vector<std::string> location_ids) {
  std::pair<double, std::vector<std::vector<std::string>>> records;
  std::vector<std::vector<std::string>> path_perm_storage;

  double min_cost = INT_MAX;
  double dist_between_nodes;

  std::vector<std::vector<double>> weights(location_ids.size(), std::vector<double>(location_ids.size(), 0.0));
  for(int i=0; i<location_ids.size(); i++){
    for(int j=0; j<location_ids.size(); j++){
      dist_between_nodes = CalculateDistance(location_ids[i], location_ids[j]);
      weights[i][j] = dist_between_nodes;
    }
  }

  std::string location_id_start = location_ids[0];
  std::vector<std::string> current_path = {location_id_start}; // start with first location
  std::vector<std::string> min_path;
  TravelingTrojan_Backtracking_Helper(location_id_start, weights, location_id_start, 0, current_path, min_cost, min_path, path_perm_storage, location_ids);  
  
  std::sort(path_perm_storage.begin(), path_perm_storage.end(), [this](std::vector<std::string> &path1, std::vector<std::string> &path2) {
        return CalculatePathLength(path1) > CalculatePathLength(path2);
  });
  
  std::cout<<min_cost<<std::endl;
  records = std::make_pair(min_cost, path_perm_storage);

  return records;
}

void TrojanMap::TravelingTrojan_Backtracking_Helper(std::string start_location, 
                                    std::vector<std::vector<double>> &weights, std::string current_node, double current_cost,
                                    std::vector<std::string> &current_path, double &min_cost, std::vector<std::string> min_path, 
                                    std::vector<std::vector<std::string>> &path_perm_storage, std::vector<std::string> location_ids){
  //if at leaf update min cost and path
  if(current_path.size() == weights.size()){
    current_path.push_back(start_location);
    double final_cost = current_cost + CalculateDistance(current_node, start_location);
    if(final_cost < min_cost){
      min_cost = final_cost;
      path_perm_storage.push_back(current_path);
    }
    current_path.pop_back();
    return;
  }

  // Early backtracking
  if(current_cost >= min_cost){
    return;
  }

  //else evaluate all children - dfs
  for(int i=0; i<weights.size(); i++){
    if(std::find(current_path.begin(), current_path.end(), location_ids[i]) != current_path.end()){
      continue;
    }
    current_path.push_back(location_ids[i]);
    TravelingTrojan_Backtracking_Helper(start_location, weights, location_ids[i], current_cost+CalculateDistance(current_node, location_ids[i]), current_path, min_cost, min_path, path_perm_storage, location_ids);  

    current_path.pop_back();
  }
}

// Hint: https://en.wikipedia.org/wiki/2-opt
std::pair<double, std::vector<std::vector<std::string>>> TrojanMap::TravelingTrojan_2opt(
      std::vector<std::string> location_ids){
  std::pair<double, std::vector<std::vector<std::string>>> records;
std::cout << "Input: " << std::endl;
  for(auto z: location_ids){
    std::cout<<z<<" ";
  }
  std::cout<<"\n"<<std::endl;
  std::vector<std::vector<std::string>> path_perm_storage; 
  double shortest_dist = INT_MAX;
  location_ids.push_back(location_ids[0]);
  path_perm_storage.push_back(location_ids);
  location_ids.pop_back();

  bool better_path = true;

  while(better_path == true){
    better_path = false;

    for(int i=1; i<location_ids.size()-1; i++){
      for(int j=i+1; j<location_ids.size(); j++){
        std::vector<std::string> alt_path = twoOptSwap(location_ids, i, j);
        double current_shortest_dist = CalculatePathLength(alt_path)+ CalculateDistance(alt_path.back(), location_ids[0]);

        if(current_shortest_dist < shortest_dist){
          alt_path.push_back(location_ids[0]);
          path_perm_storage.push_back(alt_path);
          alt_path.pop_back();
          location_ids = alt_path;
          shortest_dist = current_shortest_dist ;
          better_path = true;
        }
      }
    }
  }

  records = std::make_pair(shortest_dist, path_perm_storage);
  return records;
}

std::vector<std::string> TrojanMap::twoOptSwap(std::vector<std::string>& location_ids, int i, int j){
    std::vector<std::string> alt_path;
    for(int k=0; k<=i-1; k++){
        alt_path.push_back(location_ids[k]);
    }
    for(int k=j; k>=i; k--){
        alt_path.push_back(location_ids[k]);
    }
    for(int k=j+1; k<location_ids.size(); k++){
        alt_path.push_back(location_ids[k]);
    }
    return alt_path;
}

// This is optional
std::pair<double, std::vector<std::vector<std::string>>> TrojanMap::TravelingTrojan_3opt(
      std::vector<std::string> location_ids){
  std::pair<double, std::vector<std::vector<std::string>>> records;
  return records;
}

/**
 * Given CSV filename, it read and parse locations data from CSV file,
 * and return locations vector for topological sort problem.
 * We have provided the code for you. Please do not need to change this function.
 * Example: 
 *   Input: "topologicalsort_locations.csv"
 *   File content:
 *    Name
 *    Ralphs
 *    KFC
 *    Chick-fil-A
 *   Output: ['Ralphs', 'KFC', 'Chick-fil-A']
 * @param  {std::string} locations_filename     : locations_filename
 * @return {std::vector<std::string>}           : locations
 */
std::vector<std::string> TrojanMap::ReadLocationsFromCSVFile(
    std::string locations_filename) {
  std::vector<std::string> location_names_from_csv;
  std::fstream fin;
  fin.open(locations_filename, std::ios::in);
  std::string line, word;
  getline(fin, line);
  while (getline(fin, word)) {
    location_names_from_csv.push_back(word);
  }
  fin.close();
  return location_names_from_csv;
}

/**
 * Given CSV filenames, it read and parse dependencise data from CSV file,
 * and return dependencies vector for topological sort problem.
 * We have provided the code for you. Please do not need to change this function.
 * Example: 
 *   Input: "topologicalsort_dependencies.csv"
 *   File content:
 *     Source,Destination
 *     Ralphs,Chick-fil-A
 *     Ralphs,KFC
 *     Chick-fil-A,KFC
 *   Output: [['Ralphs', 'Chick-fil-A'], ['Ralphs', 'KFC'], ['Chick-fil-A', 'KFC']]
 * @param  {std::string} dependencies_filename     : dependencies_filename
 * @return {std::vector<std::vector<std::string>>} : dependencies
 */
std::vector<std::vector<std::string>> TrojanMap::ReadDependenciesFromCSVFile(
    std::string dependencies_filename) {
  std::vector<std::vector<std::string>> dependencies_from_csv;
  std::fstream fin;
  fin.open(dependencies_filename, std::ios::in);
  std::string line, word;
  getline(fin, line);
  while (getline(fin, line)) {
    std::stringstream s(line);
    std::vector<std::string> dependency;
    while (getline(s, word, ',')) {
      dependency.push_back(word);
    }
    dependencies_from_csv.push_back(dependency);
  }
  fin.close();
  return dependencies_from_csv;
}

/**
 * DeliveringTrojan: Given a vector of location names, it should return a
 * sorting of nodes that satisfies the given dependencies. If there is no way to
 * do it, return a empty vector.
 *
 * @param  {std::vector<std::string>} locations                     : locations
 * @param  {std::vector<std::vector<std::string>>} dependencies     : prerequisites
 * @return {std::vector<std::string>} results                       : results
 */
std::vector<std::string> TrojanMap::DeliveringTrojan(
    std::vector<std::string> &locations,
    std::vector<std::vector<std::string>> &dependencies) {
  std::vector<std::string> result;
  std::map<std::string, std::vector<std::string>> aftrloc;
  std::map<std::string, int> in_degree;
  std::queue<std::string> bfs_q;
  for(auto it = locations.begin(); it != locations.end(); it++){
    in_degree[*it] = 0;
  }
  for(auto it = dependencies.begin(); it != dependencies.end(); it++){
    for(int i = 0; i < (*it).size() - 1; i++){
      aftrloc[(*it)[i]].push_back((*it)[i + 1]);
      in_degree[(*it)[i + 1]]++;
    }
  } 
  for(auto elem : locations){
    if(in_degree[elem] == 0){
      bfs_q.push(elem);
    }
  }
  while(!bfs_q.empty()){
    auto curr = bfs_q.front();
    bfs_q.pop();
    result.push_back(curr);
    for(auto elem : aftrloc[curr]){
      if(--in_degree[elem] == 0){
        bfs_q.push(elem);
      }
    }
  }
  if(result.size() != locations.size()){
    return {};
  }
  return result;     
}

/**
 * inSquare: Give a id retunr whether it is in square or not.
 *
 * @param  {std::string} id            : location id
 * @param  {std::vector<double>} square: four vertexes of the square area
 * @return {bool}                      : in square or not
 */
bool TrojanMap::inSquare(std::string id, std::vector<double> &square) {
  double id_lat = GetLat(id);
  double id_lon = GetLon(id);
  if(id_lon>=square[0] && id_lon<=square[1] && id_lat<=square[2] && id_lat>=square[3]){
    return true;
  }
  return false;
}


/**
 * GetSubgraph: Give four vertexes of the square area, return a list of location
 * ids in the squares
 *
 * @param  {std::vector<double>} square         : four vertexes of the square
 * area
 * @return {std::vector<std::string>} subgraph  : list of location ids in the
 * square
 */
std::vector<std::string> TrojanMap::GetSubgraph(std::vector<double> &square) {
  // include all the nodes in subgraph
  std::vector<std::string> subgraph;
  for(auto it = data.begin(); it!=data.end(); it++){
    if(inSquare(it->first, square)==true){
      subgraph.push_back(it->first);
    }
    else{
      continue;
    }
  }
  return subgraph;
}

/**
 * Cycle Detection: Given four points of the square-shape subgraph, return true
 * if there is a cycle path inside the square, false otherwise.
 *
 * @param {std::vector<std::string>} subgraph: list of location ids in the
 * square
 * @param {std::vector<double>} square: four vertexes of the square area
 * @return {bool}: whether there is a cycle or not
 */
bool TrojanMap::CycleDetection(std::vector<std::string> &subgraph, std::vector<double> &square) {
  std::queue <std::pair<std::string, std::string>> bfs_queue;
  std::unordered_map <std::string, int> visited_map;

  for(auto i: subgraph){ //initialise visited map with 0 for each state
    visited_map[i] = 0;
  }

  bfs_queue.push(std::make_pair(subgraph[0], "-1")); //push first element into bfs_queue
  visited_map[subgraph[0]] = 1;
  while(bfs_queue.size()!=0){
    std::string node = bfs_queue.front().first; //get node and parent
    std::string parent = bfs_queue.front().second;
    bfs_queue.pop(); //remove first and add neighbours

    std::vector<std::string> neighbour_ids = GetNeighborIDs(node); //vector of current node's neighbours
    for(auto i: neighbour_ids){ //check each neighbour to see if exists, if so check it has  been visited
                                // if not, then +=1 to visited and push to queue
      if(visited_map.find(i) == visited_map.end()){
        continue;
      } 
      else{
        if(visited_map[i]==0){
          visited_map[i]+=1;
          bfs_queue.push({i, node});
        }
        else if(visited_map[i]==1 && parent!=i){
          return true;
        }
      }
    }
  }

  return false;
}

/**
 * FindNearby: Given a class name C, a location name L and a number r,
 * find all locations in class C on the map near L with the range of r and
 * return a vector of string ids
 *
 * @param {std::string} className: the name of the class
 * @param {std::string} locationName: the name of the location
 * @param {double} r: search radius
 * @param {int} k: search numbers
 * @return {std::vector<std::string>}: location name that meets the requirements
 */
std::vector<std::string> TrojanMap::FindNearby(std::string attributesName, std::string name, double r, int k) {
  std::vector<std::string> res;
  std::vector<std::pair<double,std::string>> same_attribute_ids;
  for(auto it=data.begin(); it!=data.end(); it++){ //get all places of the same attribute and store in order of distance from current
    for(auto elem : it->second.attributes){
      double distance = CalculateDistance(GetID(name),it->second.id);
      if(elem == attributesName && distance < r){
        same_attribute_ids.push_back({CalculateDistance(GetID(name),it->second.id), it->second.id});
      }
    }
  }
  sort(same_attribute_ids.begin(), same_attribute_ids.end());
  same_attribute_ids.erase(same_attribute_ids.begin()); //dont include itself
  int num_to_remove = same_attribute_ids.size() - k;
  for(int i=0; i<num_to_remove; i++){
    same_attribute_ids.pop_back();
  }
  for(auto j=same_attribute_ids.begin(); j!=same_attribute_ids.end(); j++){
    res.push_back(j->second);
  } 
  return res;
}

/**
 * Shortest Path to Visit All Nodes: Given a list of locations, return the shortest
 * path which visit all the places and no need to go back to the start point.
 *
 * @param  {std::vector<std::string>} input : a list of locations needs to visit
 * @return {std::vector<std::string> }      : the shortest path
 */
std::vector<std::string> TrojanMap::TrojanPath(
      std::vector<std::string> &location_names) {
  std::map<std::set<int>, double> weight_map;
  double shortest_path = INT_MAX;
  std::vector<int> index;
  std::vector<std::string> final_path;
  std::vector<std::string> res;
  for(int i = 0; i < location_names.size(); i++){
    for(int j = i; j < location_names.size(); j++){
      std::vector<std::string> path = CalculateShortestPath_Bellman_Ford(location_names[i], location_names[j]);
      double distance = CalculatePathLength(path);
      //std::cout<<"distance"<<distance<<std::endl;
      weight_map[{i, j}] = distance;
    }
  }
  for(int i = 0; i < location_names.size(); i++){
    index.push_back(i);
  }
  do {
      double current_path = 0.0;
      for(int i = 0; i < index.size() - 1; i++) {
          std::set<int> loc_pair = {index[i], index[i + 1]};
          current_path += weight_map[loc_pair];
          //std::cout<<"current_node "<< index[i]<<std::endl;
      }
      //std::cout<<"current_path "<< current_path<<std::endl;
      if(current_path < shortest_path){
        shortest_path = current_path;
        final_path.clear();
        for(auto idx : index){
          final_path.push_back(location_names[idx]);
          //std::cout<< idx << std::endl;
        }
      }
  }while(std::next_permutation(index.begin(), index.end()));
  // for(auto elem : final_path){
  //   std::cout<<"path: "<<elem<<std::endl;
  // }
  for(int i = 0; i < final_path.size() - 1; i++) {
    auto path_segment = CalculateShortestPath_Bellman_Ford(final_path[i], final_path[i + 1]);
    if (i != 0 && !path_segment.empty()) {
        path_segment.erase(path_segment.begin());
    }// remove the first point if the segment not the first.
    res.insert(res.end(), path_segment.begin(), path_segment.end());
}
return res;
}

/**
 * Given a vector of queries, find whether there is a path between the two locations with the constraint of the gas tank.
 *
 * @param  {std::vector<std::pair<double, std::vector<std::string>>>} Q : a list of queries
 * @return {std::vector<bool> }      : existence of the path
 */
std::vector<bool> TrojanMap::Queries(const std::vector<std::pair<double, std::vector<std::string>>>& q) {
    std::vector<bool> ans;
    for(auto elem : q){
      double tank_capacity = elem.first;
      std::string source = elem.second[0];  
      std::string destination = elem.second[1];
      std::string source_id = GetID(source);
      std::string destination_id = GetID(destination);
      ans.push_back(Queries_helper(source_id, destination_id, tank_capacity));
    }
    return ans;
}

/**
 * CreateGraphFromCSVFile: Read the map data from the csv file
 * We have provided the code for you. Please do not need to change this function.
 */
void TrojanMap::CreateGraphFromCSVFile() {
  // Do not change this function
  std::fstream fin;
  fin.open("src/lib/data.csv", std::ios::in);
  std::string line, word;

  getline(fin, line);
  while (getline(fin, line)) {
    std::stringstream s(line);

    Node n;
    int count = 0;
    while (getline(s, word, ',')) {
      word.erase(std::remove(word.begin(), word.end(), '\''), word.end());
      word.erase(std::remove(word.begin(), word.end(), '"'), word.end());
      word.erase(std::remove(word.begin(), word.end(), '{'), word.end());
      word.erase(std::remove(word.begin(), word.end(), '}'), word.end());
      if (count == 0)
        n.id = word;
      else if (count == 1)
        n.lat = stod(word);
      else if (count == 2)
        n.lon = stod(word);
      else if (count == 3)
        n.name = word;
      else {
        word.erase(std::remove(word.begin(), word.end(), ' '), word.end());
        if (isalpha(word[0])) n.attributes.insert(word);
        if (isdigit(word[0])) n.neighbors.push_back(word);
      }
      count++;
    }
    data[n.id] = n;
  }
  fin.close();
}
