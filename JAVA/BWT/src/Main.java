
import java.time.Instant;
import java.util.Random;

public class Main {
    /**
     * This class main method is a test bench for string search with 4 techniques:
     * Naif search
     * Java default substring search
     * BWT string search
     * Substring Trie
     */
    public static void main(String[] args) {

        char[] wholeAlphabet = {'a','b','c','d','e','f','g','h','i','j','k','i','j','k','l','m','n','o','p','q','r','s','t','u','v','w','x','y','z'};
        char[] dna = {'a','c','g','t'};

        int testTimes = 10;

        String string;
        BWTString bwt_of_string;
        SuffixTrie suffixTrie;
        int sub_length = 20000;


        for(int s_length = 600000; s_length <=600000; s_length += 5000){
            System.out.println("Length: " + s_length);
            System.out.println("Substring length: "+sub_length);
            System.out.println("Times: "+testTimes);

            //generate String and substring
            string = randomString(wholeAlphabet,s_length);
            String sub1 = string.substring(s_length-sub_length,s_length-1);

            //string = trickyString(wholeAlphabet,s_length);
            //String sub1 = string.substring(string.length()-sub_length);

            //string = trickyString2(dna,s_length);
            //String sub1 = string.substring(0,sub_length);

            //Test BWT search
            Instant baseBWT = Instant.now();
            bwt_of_string = new BWTString(string,false);
            Instant createdBWT = Instant.now();
            for(int i = 0; i < testTimes; i++){
               bwt_of_string.subStringSearch(sub1);
            }
            Instant endBWT = Instant.now();

            for(int i = 0; i < testTimes; i++){
                bwt_of_string.parallelStringSearch(sub1);
            }
            Instant endBWTpa = Instant.now();

            System.out.println("BWT done");


            //Test Naif search
            Instant baseNAIF = Instant.now();
            for(int i = 0; i < testTimes; i++){
                stringSearch(string,sub1);
            }
            Instant endNAIF = Instant.now();
            System.out.println("NAIVE done");

            //Test Java default
            Instant baseJava = Instant.now();
            for(int i = 0; i < testTimes; i++){
                string.contains(sub1);
            }
            Instant endJava = Instant.now();
            System.out.println("Java default done");


            //Test Suffix Trie
            Instant baseST = Instant.now();
            Instant createST = Instant.now();
            if(s_length > 5000){
                System.out.println("Too long for suffix trie");
            }
            else{
                suffixTrie = new SuffixTrie();
                suffixTrie.insert(string);
                createST = Instant.now();
                for(int i = 0; i < testTimes; i++){
                    suffixTrie.search(sub1);
                }
            }
            Instant endST = Instant.now();
            System.out.println("SuffixTrie done");


            //Log all recorded time

            long timeBWTCreate = createdBWT.toEpochMilli() - baseBWT.toEpochMilli();
            long timeBWTSearch = endBWT.toEpochMilli() - createdBWT.toEpochMilli();
            long timeBWTParallel = endBWTpa.toEpochMilli() - endBWT.toEpochMilli();
            long timeNAIF = endNAIF.toEpochMilli() - baseNAIF.toEpochMilli();
            long timeSTCreate = createST.toEpochMilli() - baseST.toEpochMilli();
            long timeSTSearch = endST.toEpochMilli() - createST.toEpochMilli();
            long timeDefault = endJava.toEpochMilli() - baseJava.toEpochMilli();

            System.out.println("------------------------------------");
            System.out.println("BWT creation took: "+ timeBWTCreate +" ms");
            System.out.println("BWT search took: "+ timeBWTSearch +" ms");
            System.out.println("BWT parallel search took: "+ timeBWTParallel +" ms");
            if(s_length < 5000){
                System.out.println("TRIE creation took: "+ timeSTCreate+ " ms");
                System.out.println("TRIE search took: "+ timeSTSearch +" ms");
            }
            else{
                System.out.println("Didn't even TRIEd");
            }
            System.out.println("NAIF search took: "+ timeNAIF+" ms");
            System.out.println("Java took: "+ timeDefault+"ms");
            System.out.println("-------------------------------------");
        }
    }

    /**
     * Naif substring search. (It sucks, you are better off with the default String method)
     * @param str
     * @param subString
     * @return
     */
    private static boolean stringSearch(String str, String subString){
        boolean errorFound;

        for(int i = 0; i < str.length()-subString.length();i++){
            errorFound = false;
            for(int j = 0; j < subString.length(); j++){
                if(str.charAt(i+j) != subString.charAt(j)){
                    errorFound = true;
                    break;
                }
            }
            if(!errorFound){
                return true;
            }
        }
        return false;
    }

    /**
     * Generate a random string of length "length" over alphabet "alphabet"
     * @param alphabet
     * @param length
     * @return the generated string
     */
    private static String randomString(char[] alphabet, int length){
        char[] chars = new char[length];
        Random rnd = new Random();
        for(int i = 0; i < length; i++){
            chars[i] = alphabet[rnd.nextInt(0, alphabet.length)];
        }
        return new String(chars);
    }

    /**
     * Generate a random string of length "length" over alphabet "alphabet" in a way that makes it the
     * worst case for naif substring search
     * @param alphabet
     * @param length
     * @return
     */
    private static String trickyString(char[] alphabet,int length){
        char[] chars = new char[length];
        for(int i = 0; i < length-1; i++){
            chars[i] = alphabet[1];
        }
        chars[length-1] = alphabet[2];
        return new String(chars);
    }

    /**
     * Generate a random string of length "length" over alphabet "alphabet" in a way that makes it the
     * worst case for BWT substring search
     * @param alphabet
     * @param length
     * @return
     */
    private static String trickyString2(char[] alphabet,int length){
        char[] chars = new char[length];
        chars[0] = alphabet[0];
        for(int i = 1; i < length; i++){
            chars[i] = alphabet[1];
        }
        return new String(chars);
    }
}