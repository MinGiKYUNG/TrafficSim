package app.model.tss;

public class TodVO {
    private String id;
    private String todSeq;
    private String scheduleID;
    private String fromTime;
    private String toTime;

    public String getId() {
        return id;
    }

    public void setId(String id) {
        this.id = id;
    }

    public String getTodSeq() {
        return todSeq;
    }

    public void setTodSeq(String todSeq) {
        this.todSeq = todSeq;
    }

    public String getScheduleID() {
        return scheduleID;
    }

    public void setScheduleID(String scheduleID) {
        this.scheduleID = scheduleID;
    }

    public String getFromTime() {
        return fromTime;
    }

    public void setFromTime(String fromTime) {
        this.fromTime = fromTime;
    }

    public String getToTime() {
        return toTime;
    }

    public void setToTime(String toTime) {
        this.toTime = toTime;
    }

    @Override
    public String toString(){
        StringBuilder sb = new StringBuilder();
        sb.append(id).append(" ").append(todSeq).append(" ").append(scheduleID).append(" ").
                append(fromTime).append(" ").append(toTime).append(" ");
        return sb.toString();
    }
}
