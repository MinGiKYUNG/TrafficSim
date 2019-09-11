package app.model.tss;

public class PhaseVO {
    private String state;
    private String duration;

    public String getState() {
        return state;
    }

    public void setState(String state) {
        this.state = state;
    }

    public String getDuration() {
        return duration;
    }

    public void setDuration(String duration) {
        this.duration = duration;
    }

    @Override
    public String toString(){
        StringBuilder sb = new StringBuilder();
        sb.append(state).append(" ").append(duration).append(" ");
        return sb.toString();
    }
}
