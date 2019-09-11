package app.model.connection;

public class ConnectionVO {
    private String from;
    private String to;
    private String fromLane;
    private String toLane;
    private String tl;
    private String linkIndex;

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

    public String getFromLane() {
        return fromLane;
    }

    public void setFromLane(String fromLane) {
        this.fromLane = fromLane;
    }

    public String getToLane() {
        return toLane;
    }

    public void setToLane(String toLane) {
        this.toLane = toLane;
    }

    public String getTl() {
        return tl;
    }

    public void setTl(String tl) {
        this.tl = tl;
    }

    public String getLinkIndex() {
        return linkIndex;
    }

    public void setLinkIndex(String linkIndex) {
        this.linkIndex = linkIndex;
    }

    @Override
    public String toString() {
        StringBuilder sb = new StringBuilder();
        sb.append(from).append(" ").append(to).append(" ").append(fromLane).append(" ").append(toLane).append(" ");

        if(tl != null){
            sb.append(tl).append(" ");
        }

        if(linkIndex != null){
            sb.append(linkIndex).append(" ");
        }

        return sb.toString();
    }
}
