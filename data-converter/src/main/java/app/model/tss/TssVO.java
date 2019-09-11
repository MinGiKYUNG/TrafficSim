package app.model.tss;

import java.util.List;

public class TssVO {
    private String nodeID;
    private String version;
    private String crossNo;
    private String crossName;
    private String date;
    private String signalGroup;
    private List<TodVO> todList;
    private List<ScheduleVO> scheduleList;
    private TodPlanVO todPlan;

    public String getNodeID() {
        return nodeID;
    }

    public void setNodeID(String nodeID) {
        this.nodeID = nodeID;
    }

    public String getVersion() {
        return version;
    }

    public void setVersion(String version) {
        this.version = version;
    }

    public String getCrossNo() {
        return crossNo;
    }

    public void setCrossNo(String crossNo) {
        this.crossNo = crossNo;
    }

    public String getCrossName() {
        return crossName;
    }

    public void setCrossName(String crossName) {
        this.crossName = crossName;
    }

    public String getDate() {
        return date;
    }

    public void setDate(String date) {
        this.date = date;
    }

    public String getSignalGroup() {
        return signalGroup;
    }

    public void setSignalGroup(String signalGroup) {
        this.signalGroup = signalGroup;
    }

    public List<TodVO> getTodList() {
        return todList;
    }

    public void setTodList(List<TodVO> todList) {
        this.todList = todList;
    }

    public List<ScheduleVO> getScheduleList() {
        return scheduleList;
    }

    public void setScheduleList(List<ScheduleVO> scheduleList) {
        this.scheduleList = scheduleList;
    }

    public TodPlanVO getTodPlan() {
        return todPlan;
    }

    public void setTodPlan(TodPlanVO todPlan) {
        this.todPlan = todPlan;
    }

    @Override
    public String toString(){
        StringBuilder sb = new StringBuilder();
        sb.append(nodeID).append(" ").append(version).append(" ").append(crossNo).append(" ").append(crossName).append(" ")
                .append(date).append(" ").append(signalGroup).append(" ");
        return sb.toString();
    }
}
