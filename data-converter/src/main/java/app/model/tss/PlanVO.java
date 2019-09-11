package app.model.tss;

public class PlanVO {
    private String offset;
    private String schedule;
    private String startTime;

    public String getOffset() {
        return offset;
    }

    public void setOffset(String offset) {
        this.offset = offset;
    }

    public String getSchedule() {
        return schedule;
    }

    public void setSchedule(String schedule) {
        this.schedule = schedule;
    }

    public String getStartTime() {
        return startTime;
    }

    public void setStartTime(String startTime) {
        this.startTime = startTime;
    }

    @Override
    public String toString(){
        StringBuilder sb = new StringBuilder();
        sb.append(offset).append(" ").append(schedule).append(" ").append(startTime).append(" ");
        return sb.toString();
    }
}
