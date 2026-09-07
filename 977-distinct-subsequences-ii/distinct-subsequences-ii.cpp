class Solution {
public:
    int distinctSubseqII(string s) {
        const long long MOD = 1e9 + 7;

        vector<long long> dp(26, 0);

        for (char c : s) {
            int idx = c - 'a';

            long long total = 0;

            for (int i = 0; i < 26; i++) {
                total = (total + dp[i]) % MOD;
            }

            dp[idx] = (total + 1) % MOD;
        }

        long long ans = 0;

        for (int i = 0; i < 26; i++) {
            ans = (ans + dp[i]) % MOD;
        }

        return ans;
    }
};