package app.model.edge;

public class EdgeVO {
    private String id;
    private String from;
    private String to;
    private String priority;
    private String numLanes;
    private String speed;
    private String shape;
    private String spreadType;
    private String edge_len;

    public String getId() {
        return id;
    }

    public void setId(String id) {
        this.id = id;
    }

    public String getFrom() {
        return from;
    }

    public void setFrom(String from) {
        this.from = from;
    }

    public String getTo() {
        return to;
    }

    public void setTo(String to) {
        this.to = to;
    }

    public String getPriority() {
        return priority;
    }

    public void setPriority(String priority) {
        this.priority = priority;
    }

    public String getNumLanes() {
        return numLanes;
    }

    public void setNumLanes(String numLanes) {
        this.numLanes = numLanes;
    }

    public String getSpeed() {
        return speed;
    }

    public void setSpeed(String speed) {
        this.speed = speed;
    }

    public String getShape() {
        return shape;
    }

    public void setShape(String shape) {
        this.shape = shape;
    }

    public String getSpreadType() {
        return spreadType;
    }

    public void setSpreadType(String spreadType) {
        this.spreadType = spreadType;
    }

    public String getEdge_len() {
        return edge_len;
    }

    public void setEdge_len(String edge_len) {
        this.edge_len = edge_len;
    }

    @Override
    public String toString(){
        StringBuilder sb = new StringBuilder();

        sb.append(id).append(" ").append(from).append(" ").append(to).append(" ").append(priority).append(" ")
                .append(numLanes).append(" ").append(speed).append(" ").append(shape).append(" ");

        if(spreadType != null){
            sb.append(spreadType).append(" ");
        }

        if(edge_len != null){
            sb.append(edge_len).append(" ");
        }

        return sb.toString();
    }
}
