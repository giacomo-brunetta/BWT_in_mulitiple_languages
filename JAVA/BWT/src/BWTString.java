import java.util.*;
import java.util.stream.IntStream;

/**
 * This class stores both the original String and the Burrow Wheeler Transform of a String.
 * It also  stores auxiliary structures that are needed for substring search.
 * The B-rank of characters is stored explicitly for maximum speed (rather than using the FM index technique).
 */
public class BWTString {
    String str;

    String bwt;

    int[] bRank;
    int[] fPosition;

    /**
     * Constructor. It can take the BWT and reverse it or the String and compress it in BWT
     * @param string
     * @param isBWT
     */
    BWTString(String string, boolean isBWT){
        if(isBWT){
            //Reconstruct the original string
            this.bwt = string;
            this.str = decodeToPlaintext();
            //Auxiliary structures are created as a side effect
        }
        else{
            //Compute the BWT and auxiliary structures
            this.str = string.toLowerCase();
            this.bwt = computeBWT();
            createAuxiliaryStructures();
        }
    }

    @Override
    public String toString(){
        return str;
    }

    public String getBwt(){
        return bwt;
    }

    /**
     * Compute the BWT in place
     * @return the BWT as String
     */
    private String computeBWT(){
        char[] bwt = new char[str.length()+1];
        Integer[] indexes = new Integer[str.length()+1];
        for(int i = 0; i <= str.length(); i++){
            indexes[i] = i;
        }

        //Note: Java performs a merge sort, radix sort could be faster
        Arrays.sort(indexes, this::compare);

        for(int i = 0; i <= str.length(); i++){
            if(indexes[i] == 0){
                bwt[i] = '$';
            }
            else {
                bwt[i] = str.charAt(indexes[i]-1);
            }
        }

        return new String(bwt);
    }



    /**
     * Compare two suffixes
     * @param i the starting index of the first suffix
     * @param j the starting index of the second suffix
     * @return >0 if the first is bigger, <0 if the second is bigger, 0 if equal or equivalent
     */
    private int compare(int i, int j){
        if(i == j){ //if i and j are the same the suffix is the same
            return 0;
        }
        //while in range
        while(i < str.length() && j < str.length() ){
            int diff =  str.charAt(i)-str.charAt(j);
            //compare chars
            if(diff != 0){
                return diff; //return char comparison if different
            }
            else{ //go on 1 char in both strings if equal
                i++;
                j++;
            }
        }
        //if one char is out of bounds its suffix will be lexicographically less than the other
        if (i == str.length()) {
            return -1;
        }
        return 1; // j == str.length()
    }

    /**
     * Create the bRank vector and the starting position in F column
     */
    private void createAuxiliaryStructures(){
        //build the b rank for all characters
        //count all occurrences of all characters
        bRank = new int[bwt.length()];
        fPosition = new int[26];

        char curr;

        //count all letters
        for(int i = 0; i < bwt.length(); i++){
            curr = bwt.charAt(i);
            if(curr != '$'){
                bRank[i] = fPosition[curr-'a'];
                fPosition[curr-'a']++;
            }
        }

        int offset = 1 + fPosition[0];
        fPosition[0] = 1;
        int count;

        //Convert counters into starting position on F column
        for(char c = 'b'; c <= 'z'; c++){
            count = fPosition[c-'a'];
            fPosition[c-'a'] = offset;
            offset += count;
        }
    }

    /**
     * Reverse the bwt
     * @return return the original string
     */
    private String decodeToPlaintext(){

        char[] decodedString = new char[bwt.length()-1];

        createAuxiliaryStructures();

        //actual translation
        int i = 0;
        int j = decodedString.length-1;
        while(bwt.charAt(i) != '$'){
            decodedString[j] = bwt.charAt(i);
            j--;
            i = fPosition[bwt.charAt(i)-'a']+bRank[i];
        }
        return new String(decodedString);
    }

    /**
     * Search a substring in the BWT string
     * @param subString the substring to search
     * @return true if present
     */
    public boolean subStringSearch(String subString){
        int[] range = searchRange(subString);

        //iterate through all range
        for(int i = range[0]; i < range[1]; i++){
            if(isSubStringPath(subString,i)){
                return true;
            }
        }
        return false;
    }

    /**
     * Search a substring in the BWT string (multithreaded version)
     * @param subString the substring to search
     * @return true if present
     */
    public boolean parallelStringSearch(String subString){
        int[] range = searchRange(subString);

        return  IntStream
                .range(range[0],range[1])
                .parallel()
                .anyMatch(c -> isSubStringPath(subString,c));
    }

    private int[] searchRange(String subString){
        char target = subString.charAt(subString.length()-1);

        //candidate all the letters that match the last one of the substring
        int targetBeginPos = fPosition[target-'a'];
        int targetEndPos;

        //if target is the last the range ends at the end of the array
        if(fPosition[target-'a'] == fPosition['z'-'a']){
            targetEndPos = bwt.length();
        }
        else{ //else it ends at the beginning of the consecutive letter
            targetEndPos = fPosition[target + 1 -'a'];
        }
        return new int[]{targetBeginPos,targetEndPos};
    }

    private boolean isSubStringPath(String subString, int candidate){
        int p = candidate;

        //follow path until mismatch or end of substring
        for(int i = subString.length()-2; i >= 0 ; i--){
            if(bwt.charAt(p) == subString.charAt(i)){
                p = fPosition[bwt.charAt(p)-'a']+bRank[p];
            }
            else{
                return false;
            }
        }
        return true;
    }

}