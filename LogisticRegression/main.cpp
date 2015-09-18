#include <iostream>
#include <sstream>
#include <string>
#include <vector>
#include <unordered_set>
#include <unordered_map>

using namespace std;

typedef vector<unordered_set<string> > Xtype;
typedef vector<void(*)(const string&)> ytype;

void google(const string& input) {
    cout << "Google searching " << input << endl;
}

void yahoo(const string& input) {
    cout << "Yahoo searching " << input << endl;
}

void bing(const string& input) {
    cout << "Bing searching " << input << endl;
}

unordered_set<string> word2vec(const string& input) {
    istringstream iss(input);
    string word;
    unordered_set<string> words;
    while (iss >> word) {
        words.insert(word);
    }
    return words;
}

void print_weights(const vector<unordered_map<string, float> >& weights) {
    for (int i = 0; i < weights.size(); i++) {
        cout << "Class " << i << endl;
        for (unordered_map<string, float>::const_iterator it = weights[i].begin(); it != weights[i].end(); it++) {
            cout << it->first << " : " << it->second << endl;
        }
    }
}

float sigmoid(float x) {
    return 1.0 / (1.0 + exp(-x));
}
float h_of_x(const unordered_set<string> Xi, const unordered_map<string, float> W, float w_constant) {
    float sig_sum = sigmoid(w_constant);
    for (unordered_set<string>::const_iterator word = Xi.begin(); word != Xi.end(); word++) {
        unordered_map<string, float>::const_iterator it = W.find(*word);
        if (it != W.end()) {
            sig_sum += sigmoid(it->second);
        }
    }
    return sig_sum;
}

float get_error_sum(const Xtype& X, const ytype& y, const unordered_map<string, float>& result,const string& word, float w_constant, bool Xi_const) {
    float error_sum = 0;
    for (int i = 0; i < X.size(); i++) {
        //x[i] needs to be 1 (present) OR if updating w0
        if (Xi_const || X[i].find(word) != X[i].end()) {
            float sigmoid_sum = h_of_x(X[i], result, w_constant);
            error_sum += sigmoid_sum / X[i].size();
            if (y[i] == yahoo) {
                error_sum -= 1;
            }
        }
    }
    return error_sum;
}

vector<unordered_map<string, float> > train(const Xtype& X, const ytype& y) {
    //initialize weights
    float alpha = 1.0 / X.size();
    vector<unordered_map<string, float> > result;
    float w_constant = 0.0;
    for (int i = 0; i < 1; i++) {
        result.push_back(unordered_map<string, float>());
    }
    for (int i = 0; i < X.size(); i++) {
        for (unordered_set<string>::const_iterator it = X[i].begin(); it != X[i].end(); it++) {
            result[0][*it] = 0;
        }
    }
    for (int iter_num = 0; iter_num < 1000; iter_num++) {
        vector<unordered_map<string, float> > new_result(result);
        float w_constant_error = get_error_sum(X, y, result[0], "", w_constant, true);
        float new_w_constant = w_constant - alpha * w_constant_error;
        for (unordered_map<string, float>::const_iterator feature = result[0].begin(); feature != result[0].end(); feature++) {
            float error_sum = get_error_sum(X, y, result[0], feature->first, w_constant, false); 
            new_result[0][feature->first] = result[0][feature->first] - alpha * error_sum; 
            cout << error_sum << endl;
        }
        result = new_result;
        w_constant = new_w_constant;
    }
    cout << w_constant << endl;
    return result;
}

void classify(const vector<unordered_map<string, float> >& weights, const string& sentence) {
    unordered_set<string> words = word2vec(sentence);
    float sum = 0;
    for (unordered_set<string>::const_iterator it = words.begin(); it != words.end(); it++) {
        unordered_map<string, float>::const_iterator weight_it = weights[0].find(*it);
        if (weight_it != weights[0].end()) {
            sum += weight_it->second;
        }
    }
    cout << sentence << " : " << endl;
    if (sum > 0) {
        yahoo("test");
    } else {
        google("test");
    }
}

int main(int argc, char* argv[]) {
    vector<unordered_set<string> > X;
    vector<void(*)(const string&)> y;
    X.push_back(word2vec("google starbucks"));
    y.push_back(google);
    X.push_back(word2vec("google dunkin"));
    y.push_back(google);
    X.push_back(word2vec("google whole foods"));
    y.push_back(google);
    X.push_back(word2vec("google myself"));
    y.push_back(google);


    X.push_back(word2vec("yahoo starbucks"));
    y.push_back(yahoo);
    X.push_back(word2vec("yahoo dunkin"));
    y.push_back(yahoo);
    X.push_back(word2vec("yahoo whole foods"));
    y.push_back(yahoo);
    X.push_back(word2vec("yahoo myself"));
    y.push_back(yahoo);


    vector<unordered_map<string, float> > weights = train(X, y);
    print_weights(weights);
    
    classify(weights, "google shopping");
    classify(weights, "yahoo dunkin");



    return 0;
}
