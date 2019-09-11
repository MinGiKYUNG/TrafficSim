package app.model.tss;

import java.util.List;

public class TodPlanVO {
    private String offset;
    private String defaultPlan;
    private List<PlanVO> planList;

    public String getOffset() {
        return offset;
    }

    public void setOffset(String offset) {
        this.offset = offset;
    }

    public String getDefaultPlan() {
        return defaultPlan;
    }

    public void setDefaultPlan(String defaultPlan) {
        this.defaultPlan = defaultPlan;
    }

    public List<PlanVO> getPlanList() {
        return planList;
    }

    public void setPlanList(List<PlanVO> planList) {
        this.planList = planList;
    }

    @Override
    public String toString(){
        StringBuilder sb = new StringBuilder();
        sb.append(offset).append(" ").append(defaultPlan).append(" ");
        return sb.toString();
    }
}
