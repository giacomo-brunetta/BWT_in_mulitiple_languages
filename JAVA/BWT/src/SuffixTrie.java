public class SuffixTrie extends Trie{
    Trie st = new Trie();
    SuffixTrie(){
        st = new Trie();
    }
    boolean search(String key) {
        return st.searchSubString(key);
    }

    void insert(String key) {
        int length = key.length();
        for (int i = 0; i < length; i++){
            st.insert(key.substring(i,length));
        }
    }
}
