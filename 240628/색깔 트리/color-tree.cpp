#include <iostream>
#include <vector>
using namespace std;

#define MAX_ID 100005
#define COLOR_MAX 5

int id[MAX_ID], color[MAX_ID], lastUpdate[MAX_ID], maxDepth[MAX_ID], parentId[MAX_ID], isRoot[MAX_ID];
vector<int> childIds[MAX_ID];
int cnt[COLOR_MAX+1];

bool canMakeChild(int curr, int needDepth) {
    return curr == 0 || (maxDepth[curr] > needDepth && canMakeChild(parentId[curr], needDepth+1));
}

pair<int, int> getColor(int curr) {
    if (curr == 0) return {0, 0};
    auto info = getColor(parentId[curr]);
    return info.second > lastUpdate[curr] ? info : make_pair(color[curr], lastUpdate[curr]);
}

pair<int, int*> getBeauty(int curr, int col, int lastUpd) {
    if (lastUpd < lastUpdate[curr]) { lastUpd = lastUpdate[curr]; col = color[curr]; }
    int* cnt = new int[COLOR_MAX+1]();
    cnt[col] = 1;
    int score = 0;
    for (int child : childIds[curr]) {
        auto subResult = getBeauty(child, col, lastUpd);
        for (int i=1; i<=COLOR_MAX; i++) cnt[i] += subResult.second[i];
        score += subResult.first;
        delete[] subResult.second;
    }
    int uniqueColors = 0;
    for (int i=1; i<=COLOR_MAX; i++) uniqueColors += cnt[i] ? 1 : 0;
    score += uniqueColors * uniqueColors;
    return {score, cnt};
}

int main() {
    int Q, T, mId, pId, col, maxDep;
    cin >> Q;
    for (int i=1; i<=Q; i++) {
        cin >> T;
        if (T == 100) {
            cin >> mId >> pId >> col >> maxDep;
            if (pId == -1) isRoot[mId] = 1;
            if (isRoot[mId] || canMakeChild(pId, 1)) {
                id[mId] = mId; color[mId] = col; lastUpdate[mId] = i; maxDepth[mId] = maxDep; parentId[mId] = isRoot[mId] ? 0 : pId;
                if (!isRoot[mId]) childIds[pId].push_back(mId);
            }
        } else if (T == 200) {
            cin >> mId >> col;
            color[mId] = col; lastUpdate[mId] = i;
        } else if (T == 300) {
            cin >> mId;
            cout << getColor(mId).first << endl;
        } else if (T == 400) {
            int beauty = 0;
            for (int i=1; i<MAX_ID; i++) if (isRoot[i]) beauty += getBeauty(i, color[i], lastUpdate[i]).first;
            cout << beauty << endl;
        }
    }
    return 0;
}