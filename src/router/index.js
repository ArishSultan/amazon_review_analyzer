import Vue from 'vue'
import VueRouter from 'vue-router'
import Home from '../views/Home.vue'
import CSVViewer from "../components/CSVViewer";

Vue.use(VueRouter)

  const routes = [
  {
    path: '/',
    name: 'Home',
    component: Home
  },
  {
    name: 'view-data',
    path: '/view-data',
    component: CSVViewer
  }
]

const router = new VueRouter({
  mode: 'history',
  base: process.env.BASE_URL,
  routes
})

export default router
