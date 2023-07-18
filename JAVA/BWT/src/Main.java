
import org.json.simple.JSONObject;
import org.json.simple.parser.JSONParser;
import org.json.simple.parser.ParseException;

import java.io.FileNotFoundException;
import java.io.FileReader;
import java.io.FileWriter;
import java.io.IOException;
import java.time.Instant;



public class Main {
    /**
     * This method takes a test file as an input (generate it with test_generator) and searches "test times" times
     * the substring "sub" in the string "str". It logs the time it took to search with each method
     * Naif search
     * Java default substring search
     * BWT string search
     * Substring Trie
     */
    public static void main(String[] args) {

        String testFileName = args[0]; //file name passed to main
        JSONParser parser = new JSONParser();

        long testTimes;
        String string;
        String sub;

        boolean skipped_st = false;

        try {
            //parse json file
            Object obj = parser.parse(new FileReader(testFileName));
            JSONObject jsonObject = (JSONObject) obj;

            string = (String) jsonObject.get("str");
            sub = (String) jsonObject.get("sub");
            testTimes = (long) jsonObject.get("times");

        } catch (FileNotFoundException e) {
            throw new RuntimeException("File not found");
        } catch (IOException e) {
            throw new RuntimeException("Error while reading file");
        } catch (ParseException e) {
            throw new RuntimeException("File was not in JSON format");
        }

        ///////////////////Test BWT search////////////////////////////////////

        Instant baseBWT = Instant.now();
        BWTString bwt_of_string = new BWTString(string,false);
        Instant createdBWT = Instant.now();
        for(int i = 0; i < testTimes; i++){
            bwt_of_string.subStringSearch(sub);
        }
        Instant endBWT = Instant.now();

        ///////////////////Test BWT parallel search////////////////////////////

        for(int i = 0; i < testTimes; i++){
            bwt_of_string.parallelStringSearch(sub);
        }
        Instant endBWTpa = Instant.now();

        System.out.println("BWT done");


        ///////////////////Test Naif search///////////////////////////////////

        Instant baseNAIF = Instant.now();
        for(int i = 0; i < testTimes; i++){
            stringSearch(string,sub);
        }
        Instant endNAIF = Instant.now();
        System.out.println("NAIVE done");

        ///////////////////Test String.contains() /////////////////////////////

        Instant baseJava = Instant.now();
        for(int i = 0; i < testTimes; i++){
            string.contains(sub);
        }
        Instant endJava = Instant.now();
        System.out.println("Java default done");


        /////////////////////Test Suffix Trie//////////////////////////////////

        Instant baseST = Instant.now();
        Instant createST = Instant.now();
        if(string.length() > 5000){
            skipped_st = true;
            System.out.println("Too long for suffix trie");
        }
        else{
            SuffixTrie suffixTrie = new SuffixTrie();
            suffixTrie.insert(string);
            createST = Instant.now();
            for(int i = 0; i < testTimes; i++){
                suffixTrie.search(sub);
            }
        }
        Instant endST = Instant.now();
        System.out.println("SuffixTrie done");

        /////////////////////Log all recorded times///////////////////////////////

        long timeBWTCreate = createdBWT.toEpochMilli() - baseBWT.toEpochMilli();
        long timeBWTSearch = endBWT.toEpochMilli() - createdBWT.toEpochMilli();
        long timeBWTParallel = endBWTpa.toEpochMilli() - endBWT.toEpochMilli();
        long timeNAIF = endNAIF.toEpochMilli() - baseNAIF.toEpochMilli();
        long timeSTCreate = createST.toEpochMilli() - baseST.toEpochMilli();
        long timeSTSearch = endST.toEpochMilli() - createST.toEpochMilli();
        long timeDefault = endJava.toEpochMilli() - baseJava.toEpochMilli();

        JSONObject json = new JSONObject();

        json.put("BWT_creation",timeBWTCreate);
        json.put("BWT_search",timeBWTSearch);
        json.put("BWT_search_parallel",timeBWTParallel);
        if(!skipped_st){
            json.put("ST_creation",timeSTCreate);
            json.put("ST_search",timeSTSearch);
        }
        else{
            json.put("ST_creation","Infinite");
            json.put("ST_search","Infinite");
        }
        json.put("NAIF_search",timeNAIF);
        json.put("Java_search",timeDefault);


        String filename = "test_result-" + string.length() + "-" + testTimes + "-" + sub.length() + ".json";
        try {
            FileWriter fileWriter = new FileWriter(filename);
            fileWriter.write(json.toJSONString());
            fileWriter.close();
        } catch (IOException e) {
            throw new RuntimeException(e);
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
}