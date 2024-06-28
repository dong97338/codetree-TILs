#include <iostream>
#include <vector>
#include <algorithm>
#define MAX_ID 100005
#define COLOR_MAX 5
using namespace std;

struct Node {
    int id, color, lastUpdate, maxDepth, parentId;
    vector<int> childIds;
};

class ColorCount {
public:
    int cnt[COLOR_MAX+1] = {0};
    ColorCount operator+(ColorCount const& obj) {
        ColorCount res;
        for (int i=1; i<=COLOR_MAX; i++) res.cnt[i] = cnt[i] + obj.cnt[i];
        return res;
    }
    int score() {
        int result = 0;
        for (int i=1; i<=COLOR_MAX; i++) result += cnt[i] ? 1 : 0;
        return result * result;
    }
};

Node nodes[MAX_ID];
bool isRoot[MAX_ID];

bool canMakeChild(Node curr, int needDepth) {
    return curr.id == 0 || (curr.maxDepth > needDepth && canMakeChild(nodes[curr.parentId], needDepth+1));
}

pair<int, int> getColor(Node curr) {
    if (curr.id == 0) return {0, 0};
    auto info = getColor(nodes[curr.parentId]);
    return info.second > curr.lastUpdate ? info : make_pair(curr.color, curr.lastUpdate);
}

pair<int, ColorCount> getBeauty(Node curr, int color, int lastUpdate) {
    if (lastUpdate < curr.lastUpdate) { lastUpdate = curr.lastUpdate; color = curr.color; }
    pair<int, ColorCount> result; result.second.cnt[color] = 1;
    for (int childId : curr.childIds) {
        auto subResult = getBeauty(nodes[childId], color, lastUpdate);
        result.second = result.second + subResult.second; result.first += subResult.first;
    }
    result.first += result.second.score();
    return result;
}

int main() {
    int Q, T, mId, pId, color, maxDepth;
    cin >> Q;
    for (int i=1; i<=Q; i++) {
        cin >> T;
        switch (T) {
            case 100:
                cin >> mId >> pId >> color >> maxDepth;
                if (pId == -1) isRoot[mId] = true;
                if (isRoot[mId] || canMakeChild(nodes[pId], 1)) {
                    nodes[mId] = {mId, color, i, maxDepth, isRoot[mId] ? 0 : pId};
                    if (!isRoot[mId]) nodes[pId].childIds.push_back(mId);
                }
                break;
            case 200:
                cin >> mId >> color;
                nodes[mId].color = color; nodes[mId].lastUpdate = i;
                break;
            case 300:
                cin >> mId;
                cout << getColor(nodes[mId]).first << endl;
                break;
            case 400:
                int beauty = 0;
                for (int i=1; i<MAX_ID; i++) if (isRoot[i]) beauty += getBeauty(nodes[i], nodes[i].color, nodes[i].lastUpdate).first;
                cout << beauty << endl;
                break;
        }
    }
    return 0;
}