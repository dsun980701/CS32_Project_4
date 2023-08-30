# CS32_Project4 : Unhinged
Following was uploaded for submission to MAVRL lab by David Sun.

This was final project from CS32 course in UCLA. The originally distributed project spec and provided codes are under the folder **"Unhinged_Provided"**. The folder **"Project_4"** cotains all of the submitted codes and report for the project, which were all strictly written by myself, David Sun. 

The following project has earned 100/100; I've recieved top ~4%(A+) as final grade for the course CS32.

Details can be found in the Project Spec, and the code; here is a overall high-level summary of the project:

  **Goal**
  
    -To design a text-only dating platform which matches couples based on their inputted attributes.
    
    -Matches are made based on the highest number of "compatible attributes". 
    
    -100k members are allowed to input dozens of attributes.
  
  **Restriction**
  
    1. Specific C++ Standard Template Library data sturctures, such as variants of map, are banned from usage. Specific STL limitations are found in the spec.
    
    2. Big-O notation limits are placed on data insertion and retrieval time specific to each class; most are O(log2 N) or lower than O(N) for retrieval, and O(K) for insertion where K is the length of the string. Specific Big-O limitations are found in the spec.
    
    3. Public functions cannot be added beyond the ones required in the spec.
    
  **Architecture Overview**
    -There are five classes that were implemented in the project.
    
    1. RadixTree Class 
      a. Template for map type data structure based on radixtree configuration, which is capable of mapping a key std::strings to any data type. Report document explains in great detail about the implementation logic, and the code consists of comments for the architecture.
      b. The implemented template for this data structure was widely used throughout this project, for efficient data allocation and retrieval.
      
    2. PersonProfile Class
      a. Represents a member's profile, which includes name, email, and attribute-value pairs.
      
    3. MemberDatabase Class
      a. Efficiently loads and retrieves all of the memebers' informations. All of the members' PersonProfiles are saved in RadixTree data structure.
      
    4. AttributeTranslator Class
      a. Identifies and outputs compatible attribute-value pairs based on a specified input pair.
      
    5. MatchMaker Class
      a. Based on an inputed email of a member, this class retrieves attribute-value pairs from the specified member's PersonProfile within the MemberDatabase, then determines compatibility through the AttributeTranslator, and ranks matched members based on a minimum compatibility threshold.
