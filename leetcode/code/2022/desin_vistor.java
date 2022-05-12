public class PrintCar implements Visitor {
    public void visit(Engine engine) {
        System.out.println("Visiting engine");
    }
    public void visit(Body body) {
        System.out.println("Visiting body");
    }
    public void visit(Car car) {
        System.out.println("Visiting car");
    }
}

public class Client {
    static public void main(String[] args) {
        Car car = new Car();
        car.addVisit(new Body());
        car.addVisit(new Engine());
        
        Visitor print = new PrintCar();
        car.show(print);
    }
}


作者：aoho
链接：https://juejin.cn/post/6844903582056054791
来源：稀土掘金
著作权归作者所有。商业转载请联系作者获得授权，非商业转载请注明出处。

//抽象元素：场馆景点
interface ScenerySpot {
    //接待访问者
    void accept(Visitor visitor);
    //票价（单位是分）
    Integer ticketRate();
}

//具体元素：豹子馆
class LeopardSpot implements ScenerySpot {
    @Override
    public void accept(Visitor visitor) {
        visitor.visitLeopardSpot(this);
    }

    @Override
    public Integer ticketRate() {
        //票价15元
        return 1500;
    }
}

//具体元素：海豚馆
class DolphinSpot implements ScenerySpot {
    @Override
    public void accept(Visitor visitor) {
        visitor.visitDolphinSpot(this);
    }

    @Override
    public Integer ticketRate() {
        //票价20元
        return 2000;
    }
}


//抽象访问者：游客
interface Visitor {
    //参观猎豹馆
    void visitLeopardSpot(LeopardSpot leopardSpot);
    //参观海豚馆
    void visitDolphinSpot(DolphinSpot dolphinSpot);
}

//具体的访问者：学生游客
class StudentVisitor implements Visitor {

    @Override
    public void visitLeopardSpot(LeopardSpot leopardSpot) {
        //学生票打五折
        int v = (int) (leopardSpot.ticketRate() * 0.5);
        System.out.println("学生游客游览豹子馆票价：" + v);
    }

    @Override
    public void visitDolphinSpot(DolphinSpot dolphinSpot) {
        //学生票打五折
        int v = (int) (dolphinSpot.ticketRate() * 0.5);
        System.out.println("学生游客游览海豚馆票价：" + v);
    }
}

//具体的访问者：普通游客
class CommonVisitor implements Visitor {

    @Override
    public void visitLeopardSpot(LeopardSpot leopardSpot) {
        System.out.println("普通游客游览豹子馆票价：" + leopardSpot.ticketRate());
    }

    @Override
    public void visitDolphinSpot(DolphinSpot dolphinSpot) {
        System.out.println("普通游客游览海豚馆票价：" + dolphinSpot.ticketRate());
    }
}

//对象结构角色：动物园
class Zoo {
    //场馆集合
    private List<ScenerySpot> list = new ArrayList<>();

    //接待游客
    public void accept(Visitor visitor) {
        for (ScenerySpot scenerySpot : list) {
            scenerySpot.accept(visitor);
        }
    }

    public void add(ScenerySpot scenerySpot) {
        list.add(scenerySpot);
    }

    public void remove(ScenerySpot scenerySpot) {
        list.remove(scenerySpot);
    }
}
