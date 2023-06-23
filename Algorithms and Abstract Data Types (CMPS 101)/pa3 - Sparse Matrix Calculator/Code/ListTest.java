// Name: Leonid Shuster, ID #1616143
// CruzID: lshuster
// Assignment name: pa3
// Role: Test client for list ADT

public class ListTest {
    public static void main(String[] args){
        List A = new List();
        List B = new List();

        A.append(1);
        System.out.println(A);
        A.append(2);
        System.out.println(A);
        A.append(3);
        System.out.println(A);
        A.append(5);
        System.out.println(A);

        A.prepend(4);
        System.out.println(A);
        A.prepend(2);
        System.out.println(A);
        A.prepend(1);
        System.out.println(A);

        A.moveFront();
        A.insertAfter(12);
        System.out.println(A);

        A.moveNext();
        A.insertBefore(115);
        System.out.println(A);

        A.delete();
        System.out.println(A);

        A.moveFront();
        A.deleteFront();
        System.out.println(A);

        A.moveFront();
        A.deleteBack();
        System.out.println(A);

        B.append(1);
        B.append(2);
        B.append(3);
        B.append(4);
        B.append(5);
        B.append(6);
        B.append(7);
        B.append(8);
        B.append(9);

        System.out.println(A.equals(B));




    }
}
