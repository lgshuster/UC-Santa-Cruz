// Name: Leonid Shuster, ID #1616143
// CruzID: lshuster
// Assignment name: pa3
// Role: Test client for matrix ADT

public class MatrixTest {
    public static void main(String[] args){
        int i, j, n=100;
        Matrix A = new Matrix(n);
        Matrix B = new Matrix(n);

        A.changeEntry(1, 1, 4);
        System.out.println(A);
        A.changeEntry(1, 2, 2);
        System.out.println(A);
        A.changeEntry(1, 3, 0);
        System.out.println(A);
        A.changeEntry(2, 1, 2);
        System.out.println(A);

        B = A.copy();
        System.out.println(B);
        System.out.println(A.equals(B));

        System.out.println(A.getSize());
        System.out.println(B.getSize());

        System.out.println(A.getNNZ());
        System.out.println(B.getNNZ());

        System.out.println(A.add(A));
        System.out.println(B.add(B));

        System.out.println(A.add(B));
        System.out.println(B.add(A));

        System.out.println(A.sub(A));
        System.out.println(B.sub(B));

        System.out.println(A.sub(B));
        System.out.println(B.sub(A));

        System.out.println(A.scalarMult(3));
        System.out.println(B.scalarMult(0));

        System.out.println(A.transpose());
        System.out.println(B.transpose());

        System.out.println(A.mult(B));
        System.out.println(B.mult(A));

        B.makeZero();

        System.out.println(A.add(B));
        System.out.println(A.sub(B));
        System.out.println(B.add(A));

    }
}
