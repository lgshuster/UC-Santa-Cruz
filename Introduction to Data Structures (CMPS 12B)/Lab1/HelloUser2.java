// Name: Leonid Shuster, ID #1616143
// Role: Prints out "Hello" and then user name
// File name: HelloUser2.java
// Instructions: Compile with javac HelloUser.java and then run with java HelloUser

class HelloUser2{
   public static void main( String[] args ){
      String userName = System.getProperty("user.name");
      System.out.println("Hello "+userName);
   }
}
