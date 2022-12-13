import java.util.*;

class CRC {
    static String xor(String a, String b) {
        StringBuilder stringbuilder = new StringBuilder();
        int len = Math.min(a.length(), b.length());
        for (int i = 0; i < len; i++) {
            if (a.charAt(i) == b.charAt(i)) {
                stringbuilder.append('0');
            } else {
                stringbuilder.append('1');
            }
        }
        return stringbuilder.toString();
    }

    static String divide(String dividend, String divisor) {
        int dividendLength = dividend.length();
        int divisorLength = divisor.length();
        while (dividendLength >= divisorLength) {
            String temp;
            if (dividend.charAt(0) == '1') {
                temp = xor(divisor, dividend.substring(0, divisorLength));
            } else
                temp = dividend.substring(0, divisorLength);
            dividend = temp.substring(1) + dividend.substring(divisorLength);
            dividendLength -= 1;
        }
        return dividend;
    }

    static String generateCodeWord(String message, String generator) {
        int msgLength = message.length();
        int gtrLength = generator.length();

        String dividend = String.format("%-" + (msgLength + gtrLength - 1) + "s", message).replace(' ', '0');
        String remainder = divide(dividend, generator);
        return message + remainder;
    }

    static Boolean checkCodeWord(String codeword, String generator) {
        String temp = divide(codeword, generator);
        int len = temp.length();

        for (int i = 0; i < len; i++) {
            if (temp.charAt(0) == '1') {
                return false;
            }
        }
        return true;
    }

    public static void main(String... args) {
        Scanner sc = new Scanner(System.in);
        System.out.println("Enter the message: ");
        String message = sc.nextLine();
        System.out.println("Enter the generator: ");
        String generator = sc.nextLine();
        String codeword = generateCodeWord(message, generator);
        System.out.println("Codeword: " + codeword);
        System.out.println("Enter the codeword: ");
        String codeword1 = sc.nextLine();
        if (checkCodeWord(codeword1, generator)) {
            System.out.println("There is no error");
        } else {
            System.out.println("There is error");
        }
        sc.close();
    }
}