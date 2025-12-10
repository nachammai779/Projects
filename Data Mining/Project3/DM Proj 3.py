
import numpy as np
import random
from matplotlib import pyplot as plt
random.seed(1984)
global SSEORMANHATN
SSEORMANHATN = 'S' # S for SSE and M for Manhattan

def convert_to_2d_array(points):
    points = np.array(points)
    if len(points.shape) == 1:
        points = np.expand_dims(points, -1)
    return points


def SSE(points):
    global SSEORMANHATN
    points = convert_to_2d_array(points)
    centroid = np.mean(points, 0)
    if SSEORMANHATN == 'M':
        errors = np.linalg.norm(points-centroid, ord=1, axis=1)
    else:
        errors = np.linalg.norm(points-centroid, ord=2, axis=1)
    return np.sum(errors)


def kmeans(points, k=2, epochs=10, max_iter=100, verbose=False):
    points = convert_to_2d_array(points)

    best_sse = np.inf
    for ep in range(epochs):
        # Randomly initialize k centroids
        np.random.shuffle(points)
        centroids = points[0:k, :]
        
        last_sse = np.inf
        for it in range(max_iter):
            # Cluster assignment
            clusters = [None] * k
            for p in points:
                index = np.argmin(np.linalg.norm(centroids-p, 2, 1))
                if clusters[index] is None:
                    clusters[index] = np.expand_dims(p, 0)
                else:
                    clusters[index] = np.vstack((clusters[index], p))

            # Centroid update
            centroids = [np.mean(c, 0) for c in clusters]

            # SSE calculation
            sse = np.sum([SSE(c) for c in clusters])
            gain = last_sse - sse
            if verbose:
                print((f'Epoch: {ep:3d}, Iter: {it:4d}, '
                       f'SSE: {sse:12.4f}, Gain: {gain:12.4f}'))

            # Check for improvement
            if sse < best_sse:
                best_clusters, best_sse = clusters, sse

            # Epoch termination condition
            if np.isclose(gain, 0, atol=0.00001):
                break
            last_sse = sse

    return best_clusters


def bisecting_kmeans(points, k=2, epochs=10, max_iter=100, verbose=False):
    points = convert_to_2d_array(points)
    clusters = [points]
    while len(clusters) < k:
        max_sse_i = np.argmax([SSE(c) for c in clusters])
        cluster = clusters.pop(max_sse_i)
        two_clusters = kmeans(
            cluster, k=2, epochs=epochs, max_iter=max_iter, verbose=verbose)
        clusters.extend(two_clusters)
    return clusters


def visualize_clusters(clusters):
    plt.figure()
    for cluster in clusters:
        points = convert_to_2d_array(cluster)
        if points.shape[1] < 2:
            points = np.hstack([points, np.zeros_like(points)])
        plt.plot(points[:,0], points[:,1], 'o')
    plt.show()

def intra_cluster_distance(clusters):
    n=0
    for cluster in clusters:
        n+=1
        total_sum = 0
        centroid = np.mean(cluster, 0)
        points = convert_to_2d_array(cluster)
        for i in range(len(points)):
            total_sum += np.linalg.norm(points[i] - centroid)
        print ('Cluster: ', n)
        print ('Intra cluster Distance: ', total_sum)

def inter_cluster_distance(clusters):
    m=0
    n=0
    for clusterm in clusters:
        m+=1
        pointsm = convert_to_2d_array(clusterm)
        n=0
        for clustern in clusters:
            n+=1
            pointsn = convert_to_2d_array(clustern)
            if m != n:
                mindist = np.inf
                maxdist = 0
                dist = 0
                for i in range(len(pointsm)):
                    for j in range(len(pointsn)):
                        dist = np.linalg.norm(pointsm[i] - pointsn[j])
                        if dist < mindist:
                            mindist = dist
                        if dist > maxdist:
                            maxdist = dist
                print('Inter Cluster Distances')
                print('Clusters Considered: ', m , n)
                print('Min Distance: ', mindist)
                print('Max Distance: ', maxdist)
                        

## Running the functions by generating the co-ordinates randomly
points = []
for i in range(100):
    points.append([random.randint(1,100),random.randint(1,100)])

clusters = bisecting_kmeans(points, k=2, epochs=10, max_iter=100, verbose=False)
intra_cluster_distance(clusters)
inter_cluster_distance(clusters)
visualize_clusters(clusters)
