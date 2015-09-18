#include <iostream>
#include <vector>
#include <string>
#include <assert.h>
#include <math.h>

using namespace std;

float randf() {
    return rand() / (float)RAND_MAX;
}

pair<vector<vector<float> >, vector<vector<float> > > generate_training() {
    vector<vector<float> > xdata;
    vector<vector<float> > ydata;
    int num_points = 100;
    float epsilon = float(0.0);
    for (int i = 0; i < num_points / 3; i++) {
        vector<float> w {randf() / float(3.0) + epsilon, randf() / float(3.0) + epsilon, randf() / float(3.0) + epsilon};
        xdata.push_back(w);
        vector<float> v {1, 0, 0};
        ydata.push_back(v);
    }
    epsilon = float(0.33);
    for (int i = 0; i < num_points / 3; i++) {
        vector<float> w {randf() / float(3.0) + epsilon, randf() / float(3.0) + epsilon, randf() / float(3.0) + epsilon};
        xdata.push_back(w);
        vector<float> v {0, 1, 0};
        ydata.push_back(v);
    }

    return make_pair(xdata, ydata);
}

vector<vector<float> > init_a() {
    vector<vector<float> > a;
    for (int i = 0; i < 3; i++) {
        vector<float> al {1, 0, 0, 0};
        a.push_back(al);
    }
    return a;
}

vector<vector<float> > init_delta() {
    //delta[i][0] is ignored
    vector<vector<float> > delta;
    for (int i = 0; i < 3; i++) {
        vector<float> deltal {0, 0, 0, 0};
        delta.push_back(deltal);
    }
    return delta;
}


vector<vector<vector<float> > > random_weights() {
    vector<vector<vector<float> > > weights;
    for (int i = 0; i < 3; i++) {
        vector<vector<float> >thetal;
        for (int i = 0; i < 4; i++) {
            vector<float> thetaij {randf(), randf(), randf(), randf()};
            thetal.push_back(thetaij);
        }
        weights.push_back(thetal);
    }
    return weights;
}

float sigmoid(float val) {
    return 1.0 / (1.0 + exp(-val));
}

void forward_prop(const vector<float>& xdata, const vector<vector<vector<float> > >& weights, vector<vector<float> >& a) {
    assert(xdata.size() + 1 == a[0].size());
    for (int i = 0; i < xdata.size(); i++) {
        a[0][i+1] = xdata[i];
    }
    for (int i = 1; i < a.size(); i++) {//i = destination layer
        for (int j = 1; j < a[i].size(); j++) {//j = which node in destination layer
            a[i][j] = 0;
            for (int k = 0; k < a[i-1].size(); k++) {//k = which node in source layer
                a[i][j] += a[i-1][k] * weights[i-1][k][j];
            }
            a[i][j] = sigmoid(a[i][j]);
        }
    }
}
           
void back_prop(const vector<float>& ydata, const vector<vector<vector<float> > >& weights, const vector<vector<float> >& a, vector<vector<float> >& delta) {
    int last_layer = delta.size() - 1;
    assert(ydata.size() == delta[last_layer].size() - 1);
    for (int i = 0; i < ydata.size(); i++) {
        delta[last_layer][i+1] = a[last_layer][i+1] - ydata[i];
    }
    for (int i = last_layer - 1; i >=1; i--) {//i = destination layer for calculating delta
        for (int j = 0; j < a[i].size(); j++) {//j = which node in destination layer
            delta[i][j] = 0;
            for (int k = 1; k < a[i+1].size(); k++) {//k = which node in source layer (source layer = desination layer + 1)
                delta[i][j] += weights[i][j][k] * delta[i+1][k] * a[i][j] * (1 - a[i][j]);
            }
        }
    }
}

void print_weights(const vector<vector<vector<float> > >& weights) {
    for (int i = 0; i < weights.size(); i++) {
        for (int j = 0; j < weights[i].size(); j++) {
            for (int k = 0; k < weights[i][j].size(); k++) {
                cout << weights[i][j][k] << " ";
            }
        }
    }
    cout << endl;
}

void update_D(vector<vector<vector<float> > >& D, const vector<vector<float> >& a, const vector<vector<float> >& delta) {
    for (int i = 0; i < D.size() - 1; i++) {//i = destination layer for D
        assert(D[i].size() - 1 == 2);
        for (int j = 0; j < D[i].size(); j++) {//j = which node in destination layer
            for (int k = 0; k < D[i][j].size(); k++) {//k = which node in source layer = destination layer + 1
                D[i][j][k] += a[i][j] * delta[i+1][k];
            }
        }
    }
}

void update_weights(vector<vector<vector<float> > >& weights, const vector<vector<vector<float> > >& D, int m) {
    for (int i = 0; i < D.size(); i++) {
        for (int j = 0; j < D[i].size(); j++) {
            for (int k = 1; k < D[i][j].size(); k++) {
                weights[i][j][k] = weights[i][j][k] - D[i][j][k] / float(m);
            }
        }
    }
}

float get_error(const vector<float>& al, const vector<float>& yi) {
    float sum = 0;
    for (int i = 0; i < yi.size(); i++) {
        sum += al[i+1] - yi[i];
    }
    return sum;
}

vector<vector<vector<float> > > train(const vector<vector<float> >& xdata, const vector<vector<float> > ydata) {
    vector<vector<vector<float> > > weights = random_weights();
    for (int iterations = 0; iterations < 100; iterations++) {
        vector<vector<vector<float> > > D;
        for (int i = 0; i < 3; i++) {
            D.push_back(init_delta());
        }
        float error = 0;
        for (int i = 0; i < xdata.size(); i++) {
            vector<vector<float> > a = init_a();
            forward_prop(xdata[i], weights, a);
            error += get_error(a[a.size() - 1], ydata[i]);
            vector<vector<float> > delta = init_delta();
            back_prop(ydata[i], weights, a, delta);
            update_D(D, a, delta);
        }
        update_weights(weights, D, xdata.size());
        cout << error << endl;
    }
    return weights;
}

pair<int, float> get_class(const vector<float>& vec) {
    float max_val = vec[0];
    int max_index = 0;
    for (int i = 1; i < vec.size(); i++) {
        if (vec[i] > max_val) {
            max_val = vec[i];
            max_index = i;
        }
    }
    return make_pair(max_index+1, max_val);
}

pair<int, float> classify(const vector<vector<vector<float> > >& weights, const vector<float>& xdata) {
    vector<vector<float> > a = init_a();
    forward_prop(xdata, weights, a);
    int last_layer = a.size() - 1;
    float max_val = a[last_layer][1];
    int max_index = 1;
    for (int i = 2; i < a[last_layer].size(); i++) {
        if (a[last_layer][i] > max_val) {
            max_val = a[last_layer][i];
            max_index = i;
        }
    }
    return make_pair(max_index, max_val);

}

int main() {
    pair<vector<vector<float> >, vector<vector<float> > > training = generate_training();
    vector<vector<vector<float> > > weights = train(training.first, training.second);
    //print_weights(weights);
    for (int i = 0; i < 5; i++) {
        pair<int, float> guess = classify(weights, training.first[i]);
        pair<int, float> actual = get_class(training.second[i]);
        cout << "Classification: " << guess.first << " Score: " << guess.second << " Actual: " << actual.first << endl;
    }
    for (int i = 40; i < 45; i++) {
        pair<int, float> guess = classify(weights, training.first[i]);
        pair<int, float> actual = get_class(training.second[i]);
        cout << "Classification: " << guess.first << " Score: " << guess.second << " Actual: " << actual.first << endl;
    }

    return 0;
}
