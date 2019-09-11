package app.model.tss;

import java.util.List;

public class ScheduleVO {
    private String offset;
    private String id;
    private List<PhaseVO> phaseList;

    public String getOffset() {
        return offset;
    }

    public void setOffset(String offset) {
        this.offset = offset;
    }

    public String getId() {
        return id;
    }

    public void setId(String id) {
        this.id = id;
    }

    public List<PhaseVO> getPhaseList() {
        return phaseList;
    }

    public void setPhaseList(List<PhaseVO> phaseList) {
        this.phaseList = phaseList;
    }

    @Override
    public String toString(){
        StringBuilder sb = new StringBuilder();
        sb.append(offset).append(" ").append(id).append(" ");
        return sb.toString();
    }
}
