#include "./math.h"

// Euclidean Distance Function
double GetDistance(std::vector<double>* p1, std::vector<double>* p2) {
  int size1 = p1->size();

  if (size1 != p2->size()) {
    std::cout << "Error" << std::endl;
    std::exit(0);
  }

  double distance = 0;
  for (int i = 0; i < size1; i++) {
    double diff = (*p1)[i] - (*p2)[i];
    distance += diff * diff;
  }

  return distance;
}

std::vector<double> CalculateCentroid(const Cluster& cluster) {
  std::vector<double> new_centroid;
  new_centroid.resize(cluster.points_[0].size(), 0.0);

  for (int i = 0; i < cluster.points_.size(); i++) {
    for (int j = 0; j < cluster.points_[i].size(); j++) {
      new_centroid[j] += cluster.points_[i][j];
    }
  }

  for (int i = 0; i < new_centroid.size(); i++) {
    new_centroid[i] /= cluster.points_.size();
  }

  return new_centroid;
}

double CalculateSSE(std::vector<Cluster> clusters) {
  double sse = 0.0;
  for (int i = 0; i < clusters.size(); i++) {
    for (int j = 0; j < clusters[i].points_.size(); j++) {
      sse += GetDistance(&clusters[i].points_[j], &clusters[i].centroid_);
    }
  }

  return sse;
}