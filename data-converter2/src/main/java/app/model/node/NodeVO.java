package app.model.node;

public class NodeVO {
    private String id;
    private String x;
    private String y;
    private String z;
    private String type;
    private String tl;
    private String radius;
    private String keepClear;
    private String dong_cd;
    private String partitionOwner;

    public String getId() {
        return id;
    }

    public void setId(String id) {
        this.id = id;
    }

    public String getX() {
        return x;
    }

    public void setX(String x) {
        this.x = x;
    }

    public String getY() {
        return y;
    }

    public void setY(String y) {
        this.y = y;
    }

    public String getZ() {
        return z;
    }

    public void setZ(String z) {
        this.z = z;
    }

    public String getType() {
        return type;
    }

    public void setType(String type) {
        this.type = type;
    }

    public String getTl() {
        return tl;
    }

    public void setTl(String tl) {
        this.tl = tl;
    }

    public String getRadius() {
        return radius;
    }

    public void setRadius(String radius) {
        this.radius = radius;
    }

    public String getKeepClear() {
        return keepClear;
    }

    public void setKeepClear(String keepClear) {
        this.keepClear = keepClear;
    }

    public String getDong_cd() {
        return dong_cd;
    }

    public void setDong_cd(String dong_cd) {
        this.dong_cd = dong_cd;
    }

    public String getPartitionOwner() {
        return partitionOwner;
    }

    public void setPartitionOwner(String partitionOwner) {
        this.partitionOwner = partitionOwner;
    }

    @Override
    public String toString(){
        StringBuilder sb = new StringBuilder();

        sb.append(id).append(" ").append(x).append(" ").append(y).append(" ").append(z).append(" ");

        if(type != null){
            sb.append(type).append(" ");
        }

        if(tl != null){
            sb.append(tl).append(" ");
        }

        sb.append(radius).append(" ").append(keepClear).append(" ").append(dong_cd).append(" ");

        if(partitionOwner != null){
            sb.append(partitionOwner).append(" ");
        }

        return sb.toString();
    }

}
