import java.util.ArrayList;
class Order {   // Order類別宣告
    private String orderNo;  // 成員變數
    private boolean status;
    private int pos;
    private ArrayList<OrderItem> itsItem = new ArrayList<OrderItem>();
    // 建構子: 使用參數設定成員變數初始值
    public Order(String no, boolean s) {
        orderNo = no;          // 設定編號
        status = s;            // 設定狀態
        pos = 0;               // 索引初值
    }
    // 成員方法: 新增訂單項目
    public void addItem(OrderItem item) {
        itsItem.add(item);
        pos++;
    }
    // 成員方法: 取得訂單編號
    public String getNo() { return orderNo; }
    // 成員方法: 顯示訂單資料
    public void printOrder() {
        System.out.println("訂單編號: " + orderNo);
        System.out.println("訂單狀態: " + status);
        // 顯示訂單項目
        System.out.println("編號\t數量");
        for ( int i = 0 ; i < pos; i++ )
            System.out.println(itsItem.get(i).getNo()
                    + "\t" + itsItem.get(i).getQuantity());
    }
}
class OrderItem { // OrderItem類別宣告
    private String itemNo;  // 成員變數
    private int quantity;
    // 建構子: 使用參數設定成員變數初始值
    public OrderItem(String no, int quantity) {
        itemNo = no;                 // 設定項目編號
        this.quantity = quantity;    // 設定數量
    }
    // 成員方法: 傳回編號
    public String getNo() { return itemNo; }
    // 成員方法: 傳回數量
    public int getQuantity() { return quantity; }
}
// 主程式類別
public class Ch11_4_3 {
    // 主程式
    public static void main(String[] args) {
        // 宣告物件變數且建立物件
        Order myOrder = new Order("order001", false);
        OrderItem item1 = new OrderItem("n001", 5);
        OrderItem item2 = new OrderItem("n002", 6);
        OrderItem item3 = new OrderItem("n003", 8);        
        OrderItem item4 = new OrderItem("n001", 5);
        OrderItem item5 = new OrderItem("n002", 6);
        OrderItem item6 = new OrderItem("n003", 8);
        myOrder.addItem(item1);  // 新增訂單的項目
        myOrder.addItem(item2);
        myOrder.addItem(item3);
        myOrder.addItem(item4);  // 新增訂單的項目
        myOrder.addItem(item5);
        myOrder.addItem(item6);
        myOrder.printOrder();    // 顯示訂單資料
    }
}
