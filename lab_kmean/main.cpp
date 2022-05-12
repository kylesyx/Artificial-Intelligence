#include <iostream>
#include <vector>
#include <cmath>

using namespace std;

class Point{
public:
    string key;
    float x;
    float y;
    vector<Point> cluster;

    Point(string key,float x,float y){
        this->key=key;
        this->x=x;
        this->y=y;
    };
    Point(){};
};
class Cluster{
public:
    vector<Point> points;
};

float get_euclidean_square_distance(Point a,Point b){
    float result;
    result = pow(a.x-b.x,2)+ pow(a.y-b.y,2);
    return result;
}
float get_euclidean_square_sqrt_distance(Point a,Point b){
    float result;
    result = pow(a.x-b.x,2)+ pow(a.y-b.y,2);
    result = sqrt(result);
    return result;
}
float get_manhattan_distance(Point a,Point b){
    float result;
    result = abs(a.x-b.x)+ abs(a.y-b.y);
    return result;
}
void get_cluster(vector<Point> &points,vector<Point> &centroids){
    for (int i=0;i<centroids.size();i++){
        centroids[i].cluster.clear();
    }
    for (auto a:points){
        string tempmin;
        float min=1000000000;
        for (auto b:centroids){
            if (get_euclidean_square_distance(a,b)<min){
                tempmin = b.key;
                min = get_euclidean_square_distance(a,b);
            }
        }
        for (int i=0;i<centroids.size();i++){
            if (centroids[i].key == tempmin){
                centroids[i].cluster.push_back(a);
            }
        }
    }
}
void print_cluster(vector<Point> centroids){
    for (auto a:centroids){
        cout<<a.key<<" = ";
        for (auto b:a.cluster){
            cout<<b.key;
        }
        cout<<endl;
    }
}
void recomputer_centroids(vector<Point> &centroids){
    for (int i=0;i<centroids.size();i++){
        float x_total=0;
        float y_total=0;
        if (centroids[i].cluster.size()>0){
            for (auto a:centroids[i].cluster){
                x_total+=a.x;
                y_total+=a.y;
            }
            centroids[i].x=x_total/centroids[i].cluster.size();
            centroids[i].y=y_total/centroids[i].cluster.size();
        }
        cout<<centroids[i].key<<" = ["<<centroids[i].x<<" "<<centroids[i].y<<"]"<<endl;
    }
}
void run_kmeans(vector<Point> &points,vector<Point> &centroids){
    get_cluster(points, centroids);
    print_cluster(centroids);

    recomputer_centroids(centroids);
    get_cluster(points, centroids);
    print_cluster(centroids);

    recomputer_centroids(centroids);
    get_cluster(points, centroids);
    print_cluster(centroids);

    recomputer_centroids(centroids);
    get_cluster(points, centroids);
    print_cluster(centroids);

    recomputer_centroids(centroids);
    get_cluster(points, centroids);
    print_cluster(centroids);
}

void merge_clusters(vector<Cluster>& clusters, int i, int j){
    vector<Point> j_points = clusters[j].points;
    for (int m = 0; m < j_points.size(); m++){
        clusters[i].points.push_back(j_points[m]);
    }
    clusters.erase(clusters.begin()+j);
}
void print_clusters(vector<Cluster> clusters){
    for (int i=0;i<clusters.size();i++){
        cout<<"{";
        for (int j=0;j<clusters[i].points.size();j++){
            cout<<clusters[i].points[j].key;
        }
        cout<<"}";
    }
    cout<<endl;
}
void merge_by_single_linkage(vector<Cluster>& clusters){
    int index_i;
    int index_j;
    float min_d=1000000000;
    for (int i=0;i<clusters.size();i++){
        for (int j=i+1;j<clusters.size();j++){
            vector<Point> i_points = clusters[i].points;
            vector<Point> j_points = clusters[j].points;
            int index_l;
            int index_m;
            float min_d2=1000000000;
            for (int l=0;l<i_points.size();l++) {
                for (int m = 0; m < j_points.size(); m++) {
                    if (get_euclidean_square_sqrt_distance(i_points[l],j_points[m])<min_d2){
                        index_l=l;
                        index_m=m;
                        min_d2=get_euclidean_square_sqrt_distance(i_points[l],j_points[m]);
                    }
                }
            }
            if (min_d>min_d2){
                index_i = i;
                index_j = j;
                min_d = min_d2;
            }
        }
    }
    for (int i=0;i<clusters[index_i].points.size();i++) {
        cout<<clusters[index_i].points[i].key;
    }
    cout<<" and ";
    for (int j = 0; j < clusters[index_j].points.size(); j++) {
        cout<<clusters[index_j].points[j].key;
    }
    cout<<" closest at "<<min_d<<endl;
    merge_clusters(clusters,index_i,index_j);
    print_clusters(clusters);
}
void merge_by_complete_linkage(vector<Cluster>& clusters){
    int index_i;
    int index_j;
    float min_d=1000000000;
    for (int i=0;i<clusters.size();i++){
        for (int j=i+1;j<clusters.size();j++){
            vector<Point> i_points = clusters[i].points;
            vector<Point> j_points = clusters[j].points;
            int index_l;
            int index_m;
            float max_d2=0;
            for (int l=0;l<i_points.size();l++) {
                for (int m = 0; m < j_points.size(); m++) {
                    if (get_manhattan_distance(i_points[l],j_points[m])>max_d2){
                        index_l=l;
                        index_m=m;
                        max_d2=get_manhattan_distance(i_points[l],j_points[m]);
                    }
                }
            }
            if (min_d>max_d2){
                index_i = i;
                index_j = j;
                min_d = max_d2;
            }
        }
    }
    for (int i=0;i<clusters[index_i].points.size();i++) {
        cout<<clusters[index_i].points[i].key;
    }
    cout<<" and ";
    for (int j = 0; j < clusters[index_j].points.size(); j++) {
        cout<<clusters[index_j].points[j].key;
    }
    cout<<" closest at "<<min_d<<endl;

    merge_clusters(clusters,index_i,index_j);
    print_clusters(clusters);
}

int main() {
    vector<Point> points;
    Point a = Point("A",2372,1760);
    Point b = Point("B",1656,1224);
    Point c = Point("C",603,1758);
    Point d = Point("D",2841,595);
    Point e = Point("E",2283,12);
    Point f = Point("F",829,829);
    Point g = Point("G",2879,131);
    Point h = Point("H",1496,471);
    Point i = Point("I",2978,1129);
    Point j = Point("J",611,2381);
    Point k = Point("K",785,589);
    Point l = Point("L",12,337);

    vector<Point> centroids;
    Point c1 = Point("C1",500,100);
    Point c2 = Point("C2",2000,2200);
    Point c3 = Point("C3",1000,1200);

    {
        points.push_back(a);
        points.push_back(b);
        points.push_back(c);
        points.push_back(d);
        points.push_back(e);
        points.push_back(f);
        points.push_back(g);
        points.push_back(h);
        points.push_back(i);
        points.push_back(j);
        points.push_back(k);
        points.push_back(l);
        centroids.push_back(c1);
        centroids.push_back(c2);
        centroids.push_back(c3);
    }//kmean
    run_kmeans(points,centroids);
//    vector<Cluster> clusters;
//    {
//        Cluster C1;
//        C1.points.push_back(a);
//        Cluster C2;
//        C2.points.push_back(b);
//        Cluster C3;
//        C3.points.push_back(c);
//        Cluster C4;
//        C4.points.push_back(d);
//        Cluster C5;
//        C5.points.push_back(e);
//        Cluster C6;
//        C6.points.push_back(f);
//        Cluster C7;
//        C7.points.push_back(g);
//        Cluster C8;
//        C8.points.push_back(h);
//        Cluster C9;
//        C9.points.push_back(i);
//        Cluster C10;
//        C10.points.push_back(j);
////        Cluster C11;
////        C11.points.push_back(k);
////        Cluster C12;
////        C12.points.push_back(l);
////
//        clusters.push_back(C1);
//        clusters.push_back(C2);
//        clusters.push_back(C3);
//        clusters.push_back(C4);
//        clusters.push_back(C5);
//        clusters.push_back(C6);
//        clusters.push_back(C7);
//        clusters.push_back(C8);
//        clusters.push_back(C9);
//        clusters.push_back(C10);
////        clusters.push_back(C11);
////        clusters.push_back(C12);
//    }//initialize_clusters
//    while(clusters.size()>3){
//        merge_by_complete_linkage(clusters);
//    }
    return 0;
}
