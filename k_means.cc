//Author: Jackson Rudnick
//Coding Style Standards
//https://google.github.io/styleguide/cppguide.html

/*
Overview of the steps:
Assign points to clusters 
  - Get points
  - Determine Euclidean distance to each centroid
  - Assign point to cluster with closest centroid

Evaluate quality of clusters based on SSE 
  - Calculate SSE of points in clusters

Move clusters based on mean of assigned points
  - Calculate new centroids based on means of point assigned to a cluster

Repeat
*/

#include "k_means.h"

void K_Means::AssignPointsToClusters() {
  //clear points from previous iteration
  for (int i = 0; i < clusters_.size(); i++) {
    clusters_[i].points_.clear();
  }

  //assign points to clusters O(n*k*d)
  for (int i = 0; i < num_of_points_; i++) {
    double lowest_distance = std::numeric_limits<double>::max();

    int centroid = 0;
    std::vector<double> curr_point = points_[i];

    //check distance between each point and each cluster
    for (int j = 0; j < num_of_clusters_; j++) {
      double new_distance = GetDistance(&curr_point, &clusters_[j].centroid_);
      if (new_distance < lowest_distance) {
        lowest_distance = new_distance;
        centroid = j;
      }
    }
    clusters_[centroid].points_.push_back(curr_point);

    //update worst distance of a cluster
    if (lowest_distance > clusters_[centroid].worst_distance_) {
      clusters_[centroid].worst_distance_ = lowest_distance;
      clusters_[centroid].pos_of_worst_point_ = clusters_[centroid].points_.size()-1;
    }
  }
}

bool K_Means::CalculateSSE(int iter) {
  double sse = 0.0;
  for (int i = 0; i < clusters_.size(); i++) {
    for (int j = 0; j < clusters_[i].points_.size(); j++) {
      sse += GetDistance(&clusters_[i].points_[j], &clusters_[i].centroid_);
    }
  }

  std::cout << "Iteration " << iter+1 << ": SSE = " << sse << std::endl;
  if (data_->GetConvergenceThreshold() >= (sse_ - sse)) return true;
  sse_ = sse;

  return false;
}

void K_Means::UpdateCentroids() {
  for (int i = 0; i < clusters_.size(); i++) {
    std::vector<double> new_centroid;
    new_centroid.resize(clusters_[i].centroid_.size());

    for (int j = 0; j < clusters_[i].points_.size(); j++) {
      for (int k = 0; k < clusters_[i].points_[j].size(); k++) {
        new_centroid[k] += clusters_[i].points_[j][k];
      }
    }

    for (int j = 0; j < new_centroid.size(); j++) {
      new_centroid[j] /= clusters_[i].points_.size();
    }

    clusters_[i].centroid_ = new_centroid;
  }
}

void K_Means::CheckEmptyClusters() {
  for (int i = 0; i < num_of_clusters_; i++) {
    if (clusters_[i].points_.empty()) {
      //store to reduce multiple memory accesses
      double worst_distance = 0;
      int pos_of_worst_point = -1;
      int cluster_with_worst_point = -1;

      for (int j = 0; j < num_of_clusters_; j++) {
        if (!clusters_[j].points_.empty() && clusters_[j].worst_distance_ > worst_distance) {
          worst_distance = clusters_[j].worst_distance_;
          pos_of_worst_point = clusters_[j].pos_of_worst_point_;
          cluster_with_worst_point = j;
        }
      }

      //update singleton cluster
      if (pos_of_worst_point != -1) {
        clusters_[i].points_.push_back(clusters_[cluster_with_worst_point].points_[pos_of_worst_point]);
        clusters_[i].centroid_ = clusters_[i].points_[0];

        clusters_[cluster_with_worst_point].points_.erase(clusters_[cluster_with_worst_point].points_.begin()+pos_of_worst_point);
      }
    }
  }
}

//Euclidean Distance Function
double K_Means::GetDistance(std::vector<double>* p1, std::vector<double>* p2) {
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

K_Means::K_Means(Data *data) : data_(data) {
  clusters_.resize(data->GetNumOfClusters());
  for (int i = 0; i < data->GetNumOfClusters(); i++) {
    clusters_[i].centroid_ = data->GetCentroids()[i];
  }

  //Initializing variables saves time
  num_of_points_ = data->GetNumOfPoints();
  num_of_clusters_ = data->GetNumOfClusters();
  points_ = data->GetPoints();
}

void K_Means::InitializeClusters() {
    clusters_.clear();
    clusters_.resize(num_of_clusters_);
    for (int i = 0; i < num_of_clusters_; i++)
    {
      clusters_[i].centroid_ = data_->GetCentroids()[i];
    }
}

void K_Means::Run() {
  for (int i = 0; i < data_->GetNumOfRuns(); i++) {
    sse_ = std::numeric_limits<double>::max();

    std::cout << "\nRun " << i+1 << "\n-----\n";

    data_->SelectCentroids();
    InitializeClusters();

    for (int j = 0; j < data_->GetMaxIterations(); j++) {
      #if CHECK_PERFORMANCE
      auto iter_start = std::chrono::high_resolution_clock::now();
      #endif

      AssignPointsToClusters();

      #if CHECK_PERFORMANCE
      auto iter_stop = std::chrono::high_resolution_clock::now();
      auto iter_duration = std::chrono::duration_cast<std::chrono::milliseconds>(iter_stop - iter_start);
      std::cout << "Assigning points took: " 
                << iter_duration.count() << " milliseconds" << std::endl;
      #endif

      #if CHECK_PERFORMANCE
      iter_start = std::chrono::high_resolution_clock::now();
      #endif
      
      if (CalculateSSE(j))
        break;
      
      #if CHECK_PERFORMANCE
      iter_stop = std::chrono::high_resolution_clock::now();
      iter_duration = std::chrono::duration_cast<std::chrono::milliseconds>(iter_stop - iter_start);
      std::cout << "Calculating SSE took: " 
                << iter_duration.count() << " milliseconds" << std::endl;
      #endif

      #if CHECK_PERFORMANCE
      iter_start = std::chrono::high_resolution_clock::now();
      #endif
      
      UpdateCentroids();
      
      #if CHECK_PERFORMANCE
      iter_stop = std::chrono::high_resolution_clock::now();
      iter_duration = std::chrono::duration_cast<std::chrono::milliseconds>(iter_stop - iter_start);
      std::cout << "Updating centroids took: " 
                << iter_duration.count() << " milliseconds" << std::endl;
      #endif

      #if CHECK_PERFORMANCE
      iter_start = std::chrono::high_resolution_clock::now();
      #endif

      CheckEmptyClusters();
    
      #if CHECK_PERFORMANCE
      iter_stop = std::chrono::high_resolution_clock::now();
      iter_duration = std::chrono::duration_cast<std::chrono::milliseconds>(iter_stop - iter_start);
      std::cout << "Check empty clusters took: " 
                << iter_duration.count() << " milliseconds" << std::endl;
      #endif
    }

    //keep track of best run
    if (sse_ < lowest_sse_) {
      lowest_sse_ = sse_;
      lowest_sse_run_ = i+1;
    }
  }

  std::cout << "\nBest Run: " << lowest_sse_run_ << ": SSE = " << lowest_sse_ 
    << std::endl;
}