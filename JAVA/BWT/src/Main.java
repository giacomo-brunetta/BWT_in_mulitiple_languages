
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
        boolean found = false;

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

        boolean found1 = false;

        Instant baseBWT = Instant.now();
        BWTString bwt_of_string = new BWTString(string,false);
        Instant createdBWT = Instant.now();
        for(int i = 0; i < testTimes; i++){
            found1 = bwt_of_string.subStringSearch(sub);
        }
        Instant endBWT = Instant.now();

        ///////////////////Test BWT parallel search////////////////////////////

        for(int i = 0; i < testTimes; i++){
            found = bwt_of_string.parallelStringSearch(sub);
        }
        Instant endBWTpa = Instant.now();

        System.out.println("BWT done");
        System.out.println("BWT: "+ bwt_of_string.getBwt());
        System.out.println("The substring was found: "+ found1);
        System.out.println("The substring was found: "+ found);

        ///////////////////Test String.contains() /////////////////////////////

        Instant baseJava = Instant.now();
        for(int i = 0; i < testTimes; i++){
            found = string.contains(sub);
        }
        Instant endJava = Instant.now();
        System.out.println("Java default done");
        System.out.println("The substring was found: "+ found);


        /////////////////////Test Suffix Trie//////////////////////////////////

        Instant baseST = Instant.now();
        Instant createST = Instant.now();
        if(string.length() > 5000){
            skipped_st = true;
            System.out.println("Too long for suffix trie");
            found = false;
        }
        else{
            SuffixTrie suffixTrie = new SuffixTrie();
            suffixTrie.insert(string);
            createST = Instant.now();
            for(int i = 0; i < testTimes; i++){
                found = suffixTrie.search(sub);
            }
        }
        Instant endST = Instant.now();
        System.out.println("SuffixTrie done");
        System.out.println("The substring was found: "+ found);

        /////////////////////Log all recorded times///////////////////////////////

        long timeBWTCreate = createdBWT.toEpochMilli() - baseBWT.toEpochMilli();
        long timeBWTSearch = endBWT.toEpochMilli() - createdBWT.toEpochMilli();
        long timeBWTParallel = endBWTpa.toEpochMilli() - endBWT.toEpochMilli();
        long timeSTCreate = createST.toEpochMilli() - baseST.toEpochMilli();
        long timeSTSearch = endST.toEpochMilli() - createST.toEpochMilli();
        long timeDefault = endJava.toEpochMilli() - baseJava.toEpochMilli();

        JSONObject json = new JSONObject();

        json.put("language","JAVA");
        json.put("lenght", string.length());
        json.put("sub_lenght",sub.length());
        json.put("test_times",testTimes);
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
        json.put("NAIF_search",timeDefault);

        String fileFolder = "TEST_RESULTS/";
        String filename = "test_result-JAVA-" + string.length() + "-" + testTimes + "-" + sub.length() + ".json";
        try {
            FileWriter fileWriter = new FileWriter(fileFolder+filename);
            fileWriter.write(json.toJSONString());
            fileWriter.close();
        } catch (IOException e) {
            throw new RuntimeException(e);
        }
    }
}